#include <signal.h>
#include <stdlib.h>

#include "lsubprocess.h"

void lsubprocess_destroy(lsubprocess_t *subprocess)
{
  if (subprocess->is_running == true)
    kill(subprocess->pid, SIGTERM);
  lbuffer_destroy(&subprocess->stdin);
  lbuffer_destroy(&subprocess->stdout);
  lbuffer_destroy(&subprocess->stderr);
  gtab_destroy(&subprocess->args);
  free(subprocess->dup_args);
  free(subprocess->command);
}