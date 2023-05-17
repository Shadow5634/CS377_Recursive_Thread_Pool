#include "condVar.h"

// ### MY IMPLEMENTATION START ###
  void cond_wait_usr_sleeper(int sigNum)
  {
    if (sigNum == SIGUSR1)
    {
      cout << "Handler called. Received SIGUSR1" <<  endl;
    }
    else if (sigNum == SIGUSR2)
    {
      cout << "Handler called. Received SIGUSR1" <<  endl;
    }

  }
// ### MY IMPLEMENTATION END ###

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

  sigemptyset(&(this->user_sig));
  sigaddset(&(this->user_sig), SIGUSR1);

  struct sigaction handlers;
  handlers.sa_handler = cond_wait_usr_sleeper;
  // handlers.__sigaction_handler - GIVES COMPILE TIME ERROR
  sigaction(SIGUSR1, &handlers, NULL);
  sigaction(SIGUSR2, &handlers, NULL);

  pthread_mutex_init(&(this->list_lock), NULL);
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

  struct sigaction handlers;
  handlers.sa_handler = SIG_DFL;
  sigaction(SIGUSR1, &handlers, NULL);
  sigaction(SIGUSR2, &handlers, NULL);

  pthread_mutex_destroy(&(this->list_lock));
}

// ### MY IMPLEMENTATION START ###
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

  // 1 - unlock mutex
  pthread_mutex_unlock(mutex);

  sigprocmask(SIG_BLOCK, &(this->user_sig), NULL);

  // 2 - add tid to sleeping list (it is only after this that SIGUSR1 should be sent to it)
  // this is ensured by how broadcase and signal are implemented
  // this meth should ensure that CURRENT_TID not in @sleeping_threads PREVENT DUPLICATES
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.push_front(pthread_self());
  pthread_mutex_unlock(&(this->list_lock));

  // 3 - wait until the signal arrives (blocking the signal earlier helps to ) 
  // sigwait/sigwaitinfo for suspensding execution of THREAD - returns signal number
  // sigprocmask to block/unblock singals for THREAD

  // 3 - put thread to sleep until SIGUSR1 received (ensure no locks have been acquired)
  sigwaitinfo(&(this->user_sig), NULL);

  // 4 - thread awoken, remove from sleeping list
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.remove_if(same_thread);
  pthread_mutex_unlock(&(this->list_lock));

  sigprocmask(SIG_UNBLOCK, &(this->user_sig), NULL);

  // 5 - reacquire lock
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
