#include "recurMutex.h"

using namespace std;

// ASSUMPTIONS:
// you can utilize all the POSIX thread functions
// note however that you can't use them to create recursive mutexes for you
// you must use them to create non-recursive mutexes and proceed to implement behaviour
// characteristic of recursive locks

// feel free to create as many global variables as you like
// hints
  // you need to be able distinguish between thread that has lock and those that don't
  // you need to implement a mechanism to keep track of how many times the thread has acquired the lock
  // after it has the first time
  // for globals you create think about how multiple threads accessing affects it

// initialize any global variables that you may have declared
RecursiveLock::RecursiveLock()
{
  (this->info).currThreadID = 0;
  (this->info).count = 0;
  pthread_mutex_init(&(this->info_lock), NULL);
  pthread_cond_init(&(this->sleeping_cond), NULL);
}

// free up memory, destroy locks, etc
RecursiveLock::~RecursiveLock()
{
  pthread_mutex_destroy(&(this->info_lock));
  pthread_cond_destroy(&(this->sleeping_cond));
}

// tries to unlock a mutex using recursive lock approach
int RecursiveLock::recur_mutex_try_lock()
{
  // 1 - check to see if same or different thread is trying to acquire lock
  // 2.1 - if different thread tries to lock but cannot then return -1 to signal unable to acquire lock
  // 2.2 - if same thread tries to lock then let it 'lock' and store info that thread has acquired lock n+1 times

  // -1 - lock unable to acquire
  // 0 - lock reacquired by same thread
  // 1 - lock acquired by different thread

  // note that unlike lock, both situations immediately return from the function

  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  int res;

  pthread_mutex_lock(&(this->info_lock));
    
    // recursive acquisition of a free lock by same thread
    if (this->info.currThreadID == pthread_self())
    {
      res = 0;
    }
    // acquisition of a free lock by different thread 
    else if (this->info.currThreadID == 0)
    {
      this->info.currThreadID = pthread_self();
      res = 1;
    }
    // acquisition of unfree lock by different thread - ERROR
    else
    {
      pthread_mutex_unlock(&(this->info_lock));
      return -1;
    }

    this->info.count++;
  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

// tries to unlock a mutex using recursive lock approach
int RecursiveLock::recur_mutex_unlock()
{
  // 1 - check to see if the thread trying to unlock even has the lock (note that there are 2 parts to this)
  // 2.1 - if it does not have the lock return -1
  // 2.2 - if it does have the lock, release it and store infor that thread acquired lock n-1 time
  // note that this is also a non-blocking call

  // 0 - lock unlocked by 1 level, not fully
  // 1 - were able to unlock all the way
  // -1 - could not unlock - does not possess lock

  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  // MISSING: releasing threads from sleeping if lock released all the way

  int res;

  pthread_mutex_lock(&(this->info_lock));
    // lock already free or trying to free a lock you have not acquired
    if ((this->info.currThreadID == 0) || (this->info.currThreadID != pthread_self()))
    {
      pthread_mutex_unlock(&(this->info_lock));
      return -1;
    }

    // recursive lock released all the way - it is now truly unlocked
    if (--(this->info.count) == 0)
    {
      this->info.currThreadID = 0;
      pthread_cond_signal(&(this->sleeping_cond));
      res = 1;
    }
    // recursive lock released one by level
    else
    {
      res = 0;
    }

  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

// tries to lock a mutex using recursive lock approach
int RecursiveLock::recur_mutex_lock()
{
  // 1 - check to see if same or different thread is trying to acquire lock
  // 2.1 - if different thread tries to lock then put it waiting on a condition variable
  // 2.2 - if same thread tries to lock then let it 'lock' and store info that thread has acquired lock n+1 times
  // note that this is a blocking call 

  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  pthread_mutex_lock(&(this->info_lock));
    // lock has been acquired by a different thread   
    // infinitely wait until you can assume control
    while ((this->info.currThreadID != 0) && (this->info.currThreadID != pthread_self()))
    {
      // put to sleep
      pthread_cond_wait(&(this->sleeping_cond), &(this->info_lock));
    }

    // assume control and when  made the owner, increment count:
    this->info.currThreadID = pthread_self();
    this->info.count++;
  pthread_mutex_unlock(&(this->info_lock));

  return 0;
}

// returns the #acquisitions of lock currently
int RecursiveLock::get_acqui_count()
{
  return this->info.count;
}

// returns tid of the thread occupying lock - is 0 if lock is free
pthread_t RecursiveLock::get_lock_owner()
{
  return this->info.currThreadID;
}
