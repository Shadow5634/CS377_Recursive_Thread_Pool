#ifndef RECUR_MUTEX
#define RECUR_MUTEX

  #include <stdlib.h>
  #include <string>
  #include <sys/wait.h>
  #include <semaphore.h>
  #include <assert.h>
  #include <iostream>
  #include <list>
  #include <array>
  #include <pthread.h>
  #include <unistd.h>
  #include <sys/mman.h>
  #include <fstream>

  // tries to lock a mutex using recursive lock approach
  int lock(pthread_mutex_t* mutex);

  // tries to unlock a mutex using recursive lock approach
  int unlock(pthread_mutex_t* mutex);

#endif 