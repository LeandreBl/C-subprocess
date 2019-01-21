#include "lsubprocess.h"

bool lsubprocess_is_running(lsubprocess_t *subprocess)
{
  int status;
  pid_t pid = waitpid(subprocess->pid, &status, WNOHANG);

  subprocess->is_running = (pid != 0);
  return (subprocess->is_running);
}