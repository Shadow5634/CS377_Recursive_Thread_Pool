#include "condVar.h"

/**
 * constructor to initialize class level variables
*/
ConditionVariable::ConditionVariable()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  // setting up a signal set of only SIGUSR1 signal
  // allows easy blocking on this signal using sigwaitinfo system call
  sigemptyset(&(this->user_sig));             
  sigaddset(&(this->user_sig), SIGUSR1);        

  pthread_mutex_init(&(this->list_lock), NULL); // initialize mutex
}

/**
 * deconstructor to free up memory, destroy locks, etc
*/
ConditionVariable::~ConditionVariable()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  pthread_mutex_destroy(&(this->list_lock)); // free resources for mutex
}

// ### MY IMPLEMENTATION START ###
  /**
   * checks if the thread id passed in as argument matches the thread id of the calling thread
  */
  bool same_thread(pthread_t& listEntry)
  {
    return (listEntry == pthread_self());
  }
// ### MY IMPLEMENTATION START ###

/**
 * puts the caller thread to sleep/suspends execution
 * process wakes up/resumes execution on receiving signal (SIGUSR1)
*/
void ConditionVariable::cond_var_wait(pthread_mutex_t* mutex)
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  // blocking SIGUSR1 so calling thread can wait on it using sigwaitinfo
  sigprocmask(SIG_BLOCK, &(this->user_sig), NULL);

  // add calling thread's id to sleeping list.
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.push_front(pthread_self());
  pthread_mutex_unlock(&(this->list_lock));

  // unlock mutex
  pthread_mutex_unlock(mutex);
  
  // putting thread to sleep/suspending execution until SIGUSR1 received
  sigwaitinfo(&(this->user_sig), NULL);

  // thread has awoken after receiving SIGUSR1
  // removing calling thread id from sleeping list to note that thread is no longer sleeping
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.remove_if(same_thread);
  pthread_mutex_unlock(&(this->list_lock));

  // unblocking signal since after being removed from sleeping list
  // it can't be sent SIGUSR1 through signal/broadcast functions being implemented
  sigprocmask(SIG_UNBLOCK, &(this->user_sig), NULL);

  // reacquire lock
  pthread_mutex_lock(mutex);
}

/**
 * sends wake up signal to any one of the sleeping threads (if one present)
 * return 0 if no signal (SIGUSR1) sent
 * return 1 if singal (SIGUSR1) was sent
*/
int ConditionVariable::cond_var_signal()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  int res = 0;

  pthread_mutex_lock(&(this->list_lock));

    if(this->sleeping_threads.empty() == false)
    {
      res = 1;
      pthread_kill(this->sleeping_threads.front(), SIGUSR1);
    }

  pthread_mutex_unlock(&(this->list_lock));

  return res;
}

/**
 * sends wake up signal to all of the sleeping threads (if any present)
 * return 0 if no signal (SIGUSR1) sent
 * return 1 if singals (SIGUSR1) were sent
*/
int ConditionVariable::cond_var_broadcast()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  int res = 0;

  pthread_mutex_lock(&(this->list_lock));
    auto elemItr = this->sleeping_threads.begin();

    while (elemItr != this->sleeping_threads.end())
    {
      pthread_kill(*elemItr, SIGUSR1);
      elemItr++;
      res = 1;
    }
  pthread_mutex_unlock(&(this->list_lock));

  return res;
}
