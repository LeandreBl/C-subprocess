# C-subprocess
A tiny subprocess library

```C
int main(void)
{
  lsubprocess_t sp;
  char *output;

  /* Create a "bash" subprocess, and handle the use of stdin (true) */
  lsubprocess_create(&sp, "bash", true);
  /* As stdin is handled by this program, we need to send some bytes to bash ourself */
  lsubprocess_bufferize_stdin(&sp, "ls -C -l --color=yes\nexit\n", 26);
  /* The above command just filled a buffer, we now need to send this whole buffer to the subprocess */
  lsubprocess_update_stdin(&sp);
  /* We now wait the subprocess to finish */
  lsubprocess_wait(&sp);
  /* As the process is now finished, we need to update the stdout internal buffer */
  lsubprocess_update_stdout(&sp);
  /* The buffer being now updated, we retrieve the whole stdout buffer content in a string */
  lbuffer_retrieve(&sp.stdout, &output);
  /* A little print to see if it worked */
  printf("output: %s", output);
  free(output);
  /* Destroy all internal process data, and if it was not the case, kills it with a SIGTERM */
  lsubprocess_destroy(&sp);
  return (0);
}
```
