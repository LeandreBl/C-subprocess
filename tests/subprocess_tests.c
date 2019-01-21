#include <criterion/criterion.h>

#include "lsubprocess.h"

Test(create, subprocess)
{
  lsubprocess_t sp;

  cr_assert(lsubprocess_create(&sp, "ls", false) == 0);
  lsubprocess_destroy(&sp);
  cr_assert(lsubprocess_create(&sp, "cat ../Makefile", false) == 0);
  lsubprocess_destroy(&sp);
  cr_assert(lsubprocess_create(&sp, "echo yes", false) == 0);
  lsubprocess_destroy(&sp);
}

Test(wait, subprocess)
{
  lsubprocess_t sp;

  lsubprocess_create(&sp, "sleep 1", false);
  cr_assert(lsubprocess_wait(&sp) == 0);
  lsubprocess_destroy(&sp);
}

Test(is_running, subprocess)
{
  lsubprocess_t sp;

  lsubprocess_create(&sp, "ls -C -l", false);
  cr_assert(lsubprocess_wait(&sp) == 0);
  cr_assert(lsubprocess_is_running(&sp) == false);
  lsubprocess_destroy(&sp);
}

Test(update_stdout, subprocess)
{
  lsubprocess_t sp;
  char *output;

  lsubprocess_create(&sp, "echo yo les bros c'est LRB", false);
  lsubprocess_wait(&sp);
  cr_assert(lsubprocess_update_stdout(&sp) == 22);
  lbuffer_retrieve(&sp.stdout, &output);
  cr_assert(strcmp(output, "yo les bros c'est LRB\n") == 0);
  free(output);
  lsubprocess_destroy(&sp);
}

Test(update_stderr, subprocess)
{
  lsubprocess_t sp;
  char *output;

  lsubprocess_create(&sp, "sh -c ImAStrangeUnicorn", false);
  lsubprocess_wait(&sp);
  cr_assert(lsubprocess_update_stderr(&sp) == 36);
  lbuffer_retrieve(&sp.stderr, &output);
  cr_assert(strcmp(output, "sh: 1: ImAStrangeUnicorn: not found\n") == 0);
  free(output);
  lsubprocess_destroy(&sp);
}

Test(update_and_write_stdin, subprocess)
{
  lsubprocess_t sp;
  char *output;

  lsubprocess_create(&sp, "sh", true);
  cr_assert(lsubprocess_bufferize_stdin(&sp, "echo FooBar\n", 12) == 12);
  cr_assert(lsubprocess_bufferize_stdin(&sp, "exit\n", 5) == 5);
  cr_assert(lsubprocess_update_stdin(&sp) == 17);
  lsubprocess_wait(&sp);
  lsubprocess_update_stdout(&sp);
  cr_assert(lbuffer_retrieve(&sp.stdout, &output) == 7);
  cr_assert(strcmp(output, "FooBar\n") == 0);
  free(output);
  lsubprocess_destroy(&sp);
}