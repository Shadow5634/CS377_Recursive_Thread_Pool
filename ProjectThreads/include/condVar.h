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
  #include <list>
  #include <unordered_map>

  using namespace std;

  ////////////////////////////////////////////////
  // PART 3 - Function TODOs (Class Declaration)
  ////////////////////////////////////////////////

  class ConditionVariable
  {
    private:

      list<pthread_t> sleeping_threads;
      pthread_mutex_t list_lock;

    public:
      ConditionVariable();

      ~ConditionVariable();

      int cond_var_wait(pthread_mutex_t* mutex);

      int cond_var_signal();

      int cond_var_broadcast();
  };

#endif