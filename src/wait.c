#include "lsubprocess.h"

int lsubprocess_wait(lsubprocess_t *subprocess)
{
	int status;
	pid_t pid = waitpid(subprocess->pid, &status, 0);

	if (pid != -1)
		subprocess->is_running = false;
	return (pid == -1 ? -1 : status);
}