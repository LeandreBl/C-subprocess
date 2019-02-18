#ifndef _LSUBPROCESS_H_
# define _LSUBPROCESS_H_

# include <lgtab.h>
# include <sys/wait.h>
# include <sys/cdefs.h>
# include <lbuffer.h>

typedef struct lsubprocess_s {
  char *command;
  char *dup_args;
  int fdout[2];
  int fderr[2];
  int fdin[2];
  pid_t pid;
  gtab_t args;
  lbuffer_t stdout;
  lbuffer_t stderr;
  lbuffer_t stdin;
  bool is_running : 1;
  bool use_stdin : 1;
} lsubprocess_t;

int lsubprocess_create(lsubprocess_t *subprocess, const char *command, bool use_stdin) __THROW __nonnull((1, 2));
void lsubprocess_destroy(lsubprocess_t *subprocess) __THROW __nonnull((1));

int lsubprocess_wait(lsubprocess_t *subprocess) __THROW __nonnull((1));
bool lsubprocess_is_running(lsubprocess_t *subprocess) __THROW __nonnull((1));

extern ssize_t lsubprocess_update_stdout(lsubprocess_t *subprocess) __THROW __nonnull((1));
extern ssize_t lsubprocess_update_stderr(lsubprocess_t *subprocess) __THROW __nonnull((1));
extern ssize_t lsubprocess_update_stdin(lsubprocess_t *subprocess) __THROW __nonnull((1));
extern size_t lsubprocess_bufferize_stdin(lsubprocess_t *subprocess, const void *buf, size_t count) __THROW __nonnull((1, 2));

#endif /* !_LSUBPROCESS_H_ */
