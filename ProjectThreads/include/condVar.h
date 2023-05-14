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

      list<pthread_t> sleeping_threads; // the list of sleeping threads
      pthread_mutex_t list_lock;        // lock to safely access and modify @sleeping_threads
      sigset_t user_sig;                // set of only SIGUSR1 to use for sleeping

    public:
      ConditionVariable();

      ~ConditionVariable();

      int cond_var_wait(pthread_mutex_t* mutex);

      int cond_var_signal();

      int cond_var_broadcast();
  };

#endif