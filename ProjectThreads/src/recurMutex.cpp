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

  (this->info).currThreadID = 0;                    // initially no thread has acquired lock
  (this->info).count = 0;                           // initially no thread has acquired lock
  pthread_mutex_init(&(this->info_lock), NULL);     // intitialize mutex
  pthread_cond_init(&(this->sleeping_cond), NULL);  // intitialize condition variable
}

/**
 * free up memory, destroy locks, etc
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

/**
 * tries to lock a mutex using recursive lock approach
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
