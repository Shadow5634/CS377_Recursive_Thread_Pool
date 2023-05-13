#ifndef RECUR_MUTEX
#define RECUR_MUTEX


  ////////////////////////////////////////
  // PART 1 - Important Libraries
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
  // PART 2 - Data Structure Declarations
  ////////////////////////////////////////

  typedef struct recur_lock_info
  {
    pthread_t currThreadID;
    int count;
  } recur_lock_info;

  ////////////////////////////////////////////////
  // PART 3 - Function TODOs (Class Declaration)
  ////////////////////////////////////////////////

  class RecursiveLock
  {
    private:
      recur_lock_info info;           // info about which thread holds lock and #times
      pthread_mutex_t info_lock;      // lock to access info atomically
      pthread_cond_t  sleeping_lock;  // cond to put threads to sleep to wait for lock acquisition
      pthread_mutex_t recur_lock;     // 

    public:
      // constructor to initialize info struct, initialize recursive lock etc
      RecursiveLock();

      // deconstructor to free memory/destroy locks etc
      ~RecursiveLock();

      // makes a recursive lock that shall be used by recur_mutex_lock and recur_mutex_unlock (as parameters)
      int recur_mutex_init(pthread_mutex_t* mutex);

      // destroys the created recursive lock
      int recur_mutex_destroy(pthread_mutex_t* mutex);

      // blocking function to lock a mutex using recursive lock approach
      int recur_mutex_lock(pthread_mutex_t* mutex);

      // unlocks a mutex using recursive lock approach
      int recur_mutex_unlock(pthread_mutex_t* mutex);

      // non-blocking function that tries to lock a mutex using recursive lock approach
      int recur_mutex_try_lock(pthread_mutex_t* mutex);

      int recur_unlock_NEW();

  };

#endif 