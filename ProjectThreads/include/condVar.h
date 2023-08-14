#ifndef COND_VAR
#define COND_VAR

  ////////////////////////////////////////
  // PART 1 - Important Includes
  ////////////////////////////////////////
  // do not remove any include statements, although you may add more

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
  #include <algorithm>
  #include <map>

  using namespace std;

  ////////////////////////////////////////////////
  // PART 3 - Function TODOs (Class Declaration)
  ////////////////////////////////////////////////

  typedef struct thread_info
  {
    pthread_t tid;        // thread id for which thread this info relates to
    bool sent_signal;     // whether this thread has been sent the wake-up signal
  } thread_info;

  class ConditionVariable
  {
    // declare any class variable methods that are needed (as PRIVATE)
    // used to store info such as which threads are sleeping
    // you will probably need some locks/condition variables to ensure safe access to these
    // these synchronization primitives should also be defined as class level variables
    private:
      // ### MY IMPLEMENTATION START ###
        list<pthread_t> sleeping_threads; // the list of sleeping threads
        pthread_mutex_t sleeping_list_lock;        // lock to safely access and modify @sleeping_threads
        sigset_t user_sig;                // signal set of only SIGUSR1 to use for sleeping

        std::map<pthread_t, thread_info> thread_sleep_info;
      // ### MY IMPLEMENTATION END ###

    // do not remove/change any method declarations
    // you may, however, add any declarations for any helpers that you create
    public:
      // constructor to initialize class level variables
      ConditionVariable();

      // deconstructor to free up resources used by class level variables
      ~ConditionVariable();

      // returns the number of threads currently sleeping
      int sleepingThreadCount();

      // returns as to whether the thread with id @tid is currently sleeping or not
      bool isSleeping(pthread_t tid);

      // send a singal (SIGUSR1) to any one of the sleeping threads to be woken up
      int signal();

      // send signal(SIGUSR1) to all of the sleeping threads to be woken up
      int cond_var_broadcast();

      // blocking function that suspends execution until woken up through signal/broadcast
      void cond_var_wait(pthread_mutex_t* mutex);

      void send_signal(pthread_t thread_to_wake);
  };

#endif