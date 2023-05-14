#include "../include/recurMutex.h"
// try to figure out how to not use relative paths

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

// ANSWER START

// NOTE:
// i have used two different mutex for tid and count
// this only benefits unlock however since thats the only place the two are not read togher
// both locking functions have to consider count irrespective of tid mathcing or not
// in the case of same tid, just increment ctr
// in case of diff tid check count to see if can be acquired

// it might be possible to use @currThreadID = -1 to signal that lock is free to be acquired
// in this case it acts as the mutex to be utilized for condition variables

// it does not seem to be the case but think about whether these functions must be atomic

// MIGHT WANT TO CHECK THAT INIT AND DESTROY CALLED ONLY ONCE
// ANSWER END

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
  // 2.1 - if different thread tries to lock but cannot then return -1 to signal unable to acquire locl
  // 2.2 - if same thread tries to lock then let it 'lock' and store info that thread has acquired lock n+1 times

  // -1 - lock unable to acquire
  // 0 - lock acquired by same thread
  // 1 - lock acquired by different thread

  // note that unlike lock, both situations immediately return from the function
  // later on (and immediate for 2.2) it ill acquire lock and return

  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  // should i read @currThreadID and connected together or separate
  // in another words: do i need two separate locks or is one fine

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
  // 2.1 - if it does not have the lock return error
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
    // will it be a good safety to have broadcast or signal here as well? 
    if ((this->info.currThreadID == 0) || (this->info.currThreadID != pthread_self()))
    {
      pthread_mutex_unlock(&(this->info_lock));
      return -1;
    }

    // recursive lock released all the way - it is now truly unlocked
    if (--(this->info.count) == 0)
    {
      this->info.currThreadID = 0;
      pthread_cond_broadcast(&(this->sleeping_cond)); 
      // signal should suffice, threads are all same if not acquired unlike malloc where diff threads asking for different memory
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

  // note that in 2.1 thread goes to sleep/blocked state and function does not return immediately
  // later on (and immediate for 2.2) it ill acquire lock and return

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

// THINK ABOUT THE FOLLOWING:
// when you swtitch between locks for tid and count can a different thread affect any of the globals
// i.e. read or write the wrong value
// check that the same mutex as init is passed
// need to add checks for that