#ifndef RECUR_MUTEX
#define RECUR_MUTEX


  ////////////////////////////////////////
  // PART 1 - Important Includes
  ////////////////////////////////////////

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

  ////////////////////////////////////////
  // PART 2 - Function TODOs
  ////////////////////////////////////////

  // makes a recursive lock that shall be used by recur_mutex_lock and recur_mutex_unlock (as parameters)
  int recur_mutex_init(pthread_mutex_t* mutex);

  // destroys the created recursive lock
  int recur_mutex_destroy(pthread_mutex_t* mutex);

  // tries to lock a mutex using recursive lock approach
  int recur_mutex_lock(pthread_mutex_t* mutex);

  // tries to unlock a mutex using recursive lock approach
  int recur_mutex_lock(pthread_mutex_t* mutex);

#endif 