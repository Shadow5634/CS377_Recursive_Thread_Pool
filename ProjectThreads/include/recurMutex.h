#ifndef RECUR_MUTEX
#define RECUR_MUTEX

  ////////////////////////////////////////
  // PART 1 - Important Libraries
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

  ////////////////////////////////////////
  // PART 2 - Data Structure Declarations
  ////////////////////////////////////////
  // declare any structs you may want to incorporate into the RecurLockClass

  // ### MY IMPLEMENTATION START ###
    typedef struct recur_lock_info
    {
      pthread_t currThreadID;   // thread id of current owner of recursive lock (0 if none)
      int count;                // #acquisitions of recursive lock as of now (0 if free)
    } recur_lock_info;
  // ### MY IMPLEMENTATION END ###

  ////////////////////////////////////////////////
  // PART 2 - Function TODOs (Class Declaration)
  ////////////////////////////////////////////////

  class RecursiveLock
  {
    // declare any class-level variables are needed (as PRIVATE)
    // used to store important information like lock owner, #acquisitions, etc
    // you will probably need some locks/condition variables to ensure safe access to these
    // these synchronization primitives should also be defined as class level variables
    private:
      // ### MY IMPLEMENTATION START ###
        recur_lock_info info;           // info about which thread holds lock and #times
        pthread_mutex_t info_lock;      // lock to access info atomically
        pthread_cond_t  sleeping_cond;  // cond to put threads to sleep to wait for lock acquisition
      // ### MY IMPLEMENTATION END ###

    // do not remove/change any method declarations
    // you may, however, add any declarations for any helpers that you create
    public:
      // constructor to initialize class level variables
      RecursiveLock();

      // deconstructor to free up resources used up by class level variables
      ~RecursiveLock();

      // returns the current #acquisitions of the recursive lock
      int get_acqui_count();

      // returns the thread id of the current owner of the recursive lock
      pthread_t get_lock_owner();

      bool isOwner(pthread_t tid);

      // non-blocking function that tries to acquire recursive lock
      int recur_mutex_try_lock();

      // blocking function to acquire recursive lock
      int recur_mutex_lock();

      // unlocks the recursive lock
      int recur_mutex_unlock();
  };

#endif 