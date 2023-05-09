#include "../include/recurMutex.h"

using namespace std;

// feel free to create as many global variables as you like
// hints
  // you need to be able distinguish between thread that has lock and those that don't
  // you need to implement a mechanism to keep track of how many times the thread has acquired the lock
  // after it has the first time

pthread_t currThreadID; // the thread that is currently holding the lock being trying to lock
pthread_mutex_t tid_lock; // lock dealing with @currThreadID and @proc_id

int count; // count for how many times the thread has acquired the lock
pthread_mutex_t count_lock; // lock dealing with count

// makes a recursive lock that shall be used by recur_mutex_lock and recur_mutex_unlock (as parameters)
int recur_mutex_init(pthread_mutex_t* mutex)
{
  return 0;
}

// destroys the created recursive lock
int recur_mutex_destroy(pthread_mutex_t* mutex)
{
  return 0;
}

// tries to lock a mutex using recursive lock approach
int recur_mutex_lock(pthread_mutex_t* mutex)
{
  // 1 - check to see if same or different thread is trying to acquire lock
  // 2.1 - if different thread tries to lock then put it waiting on a condition variable
  // 2.2 - if same thread tries to lock then let it 'lock' and store info that thread has acquired lock n+1 times

  // note that in 2.1 thread goes to sleep/blocked state and function does not return
  return 0;
}

// tries to unlock a mutex using recursive lock approach
int recur_mutex_unlock(pthread_mutex_t* mutex)
{
  return 0;
}

// tries to unlock a mutex using recursive lock approach
int recur_mutex_try_lock(pthread_mutex_t* mutex)
{
  return 0;
}