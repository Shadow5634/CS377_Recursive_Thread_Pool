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
      pthread_cond_t  sleeping_cond;  // cond to put threads to sleep to wait for lock acquisition

    public:
      // constructor to initialize info struct, initialize recursive lock etc
      RecursiveLock();

      // deconstructor to free memory/destroy locks etc
      ~RecursiveLock();

      // blocking function to lock a mutex using recursive lock approach
      int recur_mutex_lock();

      // unlocks a mutex using recursive lock approach
      int recur_mutex_unlock();

      // non-blocking function that tries to lock a mutex using recursive lock approach
      int recur_mutex_try_lock();

  };

#endif 