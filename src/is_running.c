#include "lsubprocess.h"

bool lsubprocess_is_running(lsubprocess_t *subprocess)
{
  int status;
  pid_t pid = waitpid(subprocess->pid, &status, WNOHANG);

  subprocess->is_running = (pid == subprocess->pid);
  return (subprocess->is_running);
}