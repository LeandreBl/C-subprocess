#include "lsubprocess.h"

inline ssize_t lsubprocess_update_stdout(lsubprocess_t *subprocess)
{
  return (lbuffer_fdwrite(&subprocess->stdout, subprocess->fdout[0], -1));
}

inline ssize_t lsubprocess_update_stderr(lsubprocess_t *subprocess)
{
  return (lbuffer_fdwrite(&subprocess->stderr, subprocess->fderr[0], -1));
}

inline size_t lsubprocess_bufferize_stdin(lsubprocess_t *subprocess, const void *buf, size_t count)
{
  return (lbuffer_write(&subprocess->stdin, buf, count));
}

inline ssize_t lsubprocess_update_stdin(lsubprocess_t *subprocess)
{
  return (lbuffer_fdread(&subprocess->stdin, subprocess->fdin[1], -1));
}