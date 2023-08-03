#include "recurMutex.h"

using namespace std;

/**
 * constructor to initialize class level variables
*/
RecursiveLock::RecursiveLock()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  (this->info).count = 0;                           // initially no thread has acquired lock
  pthread_mutex_init(&(this->info_lock), NULL);     // intitialize mutex
  pthread_cond_init(&(this->sleeping_cond), NULL);  // intitialize condition variable
}

/**
 * deconstructor to free up memory, destroy locks, etc
*/
RecursiveLock::~RecursiveLock()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  pthread_mutex_destroy(&(this->info_lock));    // free resources for mutex
  pthread_cond_destroy(&(this->sleeping_cond)); // free resources for condition variable
}

recur_lock_info RecursiveLock::get_info()
{
  recur_lock_info res;

  pthread_mutex_lock(&(this->info_lock));
    res = this->info;
  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

/**
 * returns the #acquisitions of lock currently
*/
int RecursiveLock::get_acqui_count()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  // locked here despite this function being a simple return in order to
  // avoid reading value while another thread could be modifying it
  pthread_mutex_lock(&(this->info_lock));
    int res = this->info.count;
  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

/**
 * returns the thread id of the thread that has current acquired the recursive lock
 * return 0 if the lock is free
*/
pthread_t RecursiveLock::get_lock_owner()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  pthread_t res;

  // locked here despite this function being a simple return in order to
  // avoid reading value while another thread could be modifying it
  pthread_mutex_lock(&(this->info_lock));

    if (this->info.count == 0)
    {
      res = 0;
    }
    else
    {
      res = this->info.currThreadID;
    }

  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

/**
 * Tries to lock a mutex using recursive lock approach.
 * 
 * return -1: lock unable to be acquired
 * return 0:  lock has been succesfully reacquired (calling thread is current owner)
 * return 1:  lock has been succesfully acquired (calling thread is new owner)
*/
int RecursiveLock::recur_mutex_try_lock()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  int res;  // variable that keeps track of return value

  pthread_mutex_lock(&(this->info_lock));
    
    // lock acquired by another thread
    if ((this->info.count > 0) && (pthread_equal(this->info.currThreadID, pthread_self()) == 0))
    {
      pthread_mutex_unlock(&(this->info_lock));
      return -1;
    }
    // acquisition of free lock by new owner thread
    else if (this->info.count == 0)
    {
      this->info.currThreadID = pthread_self();
      res = 1;
    }
    // reacquisition of lock by owner thread
    else
    {
      res = 0;
    }

    // incrementing count to reflect the fact that an acquisition has taken place
    this->info.count++;
  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

/**
 * Locks a mutex using recursive lock approach
 * 
 * return 0:  lock has been succesfully reacquired (calling thread is current owner)
 * return 1:  lock has been succesfully acquired (calling thread is new owner)
*/
int RecursiveLock::recur_mutex_lock()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  int res = 0; // variable that keeps track of return value

  // TODO: Decide whether you want a timeout

  pthread_mutex_lock(&(this->info_lock));
    // recursive lock has been acquired by a different thread   
    // infinitely wait until you can assume control
    while ((this->info.count > 0) && (pthread_equal(this->info.currThreadID, pthread_self()) == 0))
    {
      pthread_cond_wait(&(this->sleeping_cond), &(this->info_lock));
    }
    // new owner of recursive lock
    if (this->info.count == 0)
    {
      this->info.currThreadID = pthread_self();
      res = 1;
    }

    // either acquired or reacquired - count updated
    this->info.count++;
  pthread_mutex_unlock(&(this->info_lock));

  return res;
}

/**
 * tries to unlock a mutex using recursive lock approach
 * 
 * return -1: lock not acquired by calling thread
 * return 0:  lock has been succesfully unlocked by 1 layer
 * return 1:  lock has been succesfully unlocked fully i.e. open to acquisition by other threads
*/
int RecursiveLock::recur_mutex_unlock()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  int res; // variable that keeps track of return value

  // TODO: Decide whether you want different return values for free lock vs locked by different thread

  pthread_mutex_lock(&(this->info_lock));
    // lock already free or acquired by a different thread
    if ((this->info.count == 0) || (pthread_equal(this->info.currThreadID, pthread_self()) == 0))
    {
      pthread_mutex_unlock(&(this->info_lock));
      return -1;
    }
    // recursive lock released all the way and is open to acquisition by other threads
    // sends signal to wake up a thread that is sleeping in lock function waiting for change to acquire lock
    if (--(this->info.count) == 0)
    {
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
