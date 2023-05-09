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

pthread_t currThreadID; // the thread that is currently holding the lock being trying to lock
                        // 0 - no thread has the lock, it can be acquired, other values are actual tids 
pthread_mutex_t tid_lock; // lock dealing with @currThreadID and @proc_id
pthread_cond_t sleep_cond; // condition variable that deals with sleeping if lock is busy

int count; // count for how many times the thread has acquired the lock
pthread_mutex_t count_lock; // lock dealing with count

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
// ANSWER END

// makes a recursive lock that shall be used by recur_mutex_lock and recur_mutex_unlock (as parameters)
int recur_mutex_init(pthread_mutex_t* mutex)
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================
  return 0;
}

// destroys the created recursive lock
int recur_mutex_destroy(pthread_mutex_t* mutex)
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================
  return 0;
}

// tries to unlock a mutex using recursive lock approach
int recur_mutex_try_lock(pthread_mutex_t* mutex)
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

  pthread_mutex_lock(&tid_lock);
    
    // recursive acquisition of a free lock by same thread
    if (currThreadID == pthread_self())
    {
      pthread_mutex_lock(&count_lock);
        count++;
      pthread_mutex_unlock(&count_lock);

      res = 0;
    }
    // recursive acquisition of a free lock by different thread 
    else if (currThreadID == 0)
    {
      currThreadID = pthread_self();
      pthread_mutex_lock(&count_lock);
        count = 1;
      pthread_mutex_unlock(&count_lock);

      res = 1;
    }
    else
    {
      res = -1;
    }

  pthread_mutex_unlock(&tid_lock);

  return res;
}

// tries to unlock a mutex using recursive lock approach
int recur_mutex_unlock(pthread_mutex_t* mutex)
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

  pthread_mutex_lock(&tid_lock);
    // lock already free
    // will it be a good safety to have broadcast or signal here as well? 
    if (currThreadID == 0)
    {
      res = -1;
    }
    else
    {
      pthread_mutex_lock(&count_lock);

        // recursive lock released all the way - it is now truly unlocked
        if (--count = 0)
        {
          currThreadID = 0;
          res = 1;
        }
        // recursive lock released one by level
        else
        {
          res = 0;
        }

      pthread_mutex_unlock(&count_lock);
    }
  pthread_mutex_unlock(&tid_lock);

  return res;
}

// tries to lock a mutex using recursive lock approach
int recur_mutex_lock(pthread_mutex_t* mutex)
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

  return 0;
}