#ifndef COND_VAR
#define COND_VAR

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
  #include <signal.h>

  ////////////////////////////////////////
  // PART 2 - Function TODOs
  ////////////////////////////////////////

  int cond_var_init(pthread_cond_t* condVar);

  int cond_var_destroy(pthread_cond_t* condVar);

  int cond_var_wait(pthread_cond_t* condVar, pthread_mutex_t* mutex);

  int cond_var_signal(pthread_cond_t* condVar);

  int cond_var_broadcast(pthread_cond_t* condVar);

#endif