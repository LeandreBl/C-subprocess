#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <lstr.h>

#include "lsubprocess.h"

static char *get_path(const char *command)
{
  const char *env = secure_getenv("PATH");
  char *renv;
  char *token;
  lstr_t str = {0};

  if (env == NULL)
    return (NULL);
  renv = strdup(env);
  if (renv == NULL)
    return (NULL);
  token = strtok(renv, ":");
  while (token != NULL) {
    if (lstr_format(&str, "%s/%s", token, command) == -1 || access(str.i, R_OK | X_OK) == 0) {
      free(renv);
      return (str.i);
    }
    lstr_clear(&str);
    token = strtok(NULL, ":");
  }
  free(renv);
  return (NULL);
}

static int args_to_tab(gtab_t *tab, char *command)
{
  char *token;

  token = strtok(command, " ");
  while (token != NULL) {
    if (gtab_append(tab, token) == -1)
      return (-1);
    token = strtok(NULL, " ");
  }
  return (gtab_append(tab, NULL));
}

static int internal_allocations(lsubprocess_t *subprocess, const char *command)
{
  subprocess->dup_args = strdup(command);
  if (subprocess->dup_args == NULL || gtab_create(&subprocess->args, 10, NULL) == -1
      || args_to_tab(&subprocess->args, subprocess->dup_args) == -1
      || lbuffer_create(&subprocess->stdin, 4096) == -1
      || lbuffer_create(&subprocess->stdout, 4096) == -1
      || lbuffer_create(&subprocess->stderr, 4096) == -1)
    return (-1);
  subprocess->command = get_path(subprocess->args.i[0]);
  return (subprocess->command == NULL ? -1 : 0);
}

static int pipe_subprocess(lsubprocess_t *subprocess)
{
  if ((subprocess->use_stdin == true && pipe2(subprocess->fdin, O_CLOEXEC | O_NONBLOCK) == -1)
      || pipe2(subprocess->fdout, O_CLOEXEC | O_NONBLOCK) == -1
      || pipe2(subprocess->fderr, O_CLOEXEC | O_NONBLOCK) == -1)
    return (-1);
  return (0);
}

static int fork_and_execve(lsubprocess_t *subprocess)
{
  subprocess->pid = fork();
  if (subprocess->pid != 0) {
    if (subprocess->use_stdin == true)
      close(subprocess->fdin[0]);
    close(subprocess->fdout[1]);
    close(subprocess->fderr[1]);
    return (subprocess->pid == -1 ? -1 : 0);
  }
  if ((subprocess->use_stdin == true && dup2(subprocess->fdin[0], 0) == -1)
      || dup2(subprocess->fdout[1], 1) == -1 || dup2(subprocess->fderr[1], 2) == -1) {
    fprintf(stderr, "Failed to execute subprocess: dup2(): %s.\n", strerror(errno));
    return (-1);
  }
  if (subprocess->use_stdin == true)
    close(subprocess->fdin[1]);
  close(subprocess->fdout[0]);
  close(subprocess->fderr[0]);
  return (execve(subprocess->command, (char **)subprocess->args.i, environ));
}

int lsubprocess_create(lsubprocess_t *subprocess, const char *command, bool use_stdin)
{
  subprocess->use_stdin = use_stdin;
  if (internal_allocations(subprocess, command) == -1 || pipe_subprocess(subprocess) == -1)
    return (-1);
  return (fork_and_execve(subprocess));
}