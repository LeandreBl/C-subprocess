#include <signal.h>

#include "lsubprocess.h"

int lsubprocess_send_signal(lsubprocess_t *subprocess, int signum)
{
	if (subprocess->is_running == true)
		return (kill(subprocess->pid, signum));
	return (-1);
}