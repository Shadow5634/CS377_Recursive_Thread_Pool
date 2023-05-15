#include "condVar.h"

// 1 - pause, 2 - nanosleep, 3 - sched_yield
// might want to check out sigwaitinfo and sigprocmask
// need to research more about blocked and non-blocked signals
// for now consider pause
// you will need a variable to confirm that the condVar for init and the one used for the other methods is the same
// you will need a singal handler for SIGUSR1 [default is to terminate which you don't want to occur]
// you could incorporate a bounded buffer here
// since you will need to keep track of which threads are sleeping on the condition to wake them up
  // this will require additional condition variables and mutexes

// need to ensure no other signals can interrupt
// how to check if a thread is sleeping
// need to figure out how signal can wake up random number of threads

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

ConditionVariable::ConditionVariable()
{
  sigemptyset(&(this->user_sig));
  sigaddset(&(this->user_sig), SIGUSR1);

  struct sigaction handlers;
  handlers.sa_handler = cond_wait_usr_sleeper;
  // handlers.__sigaction_handler - GIVES COMPILE TIME ERROR
  sigaction(SIGUSR1, &handlers, NULL);
  sigaction(SIGUSR2, &handlers, NULL);

  pthread_mutex_init(&(this->list_lock), NULL);
}

ConditionVariable::~ConditionVariable()
{
  struct sigaction handlers;
  handlers.sa_handler = SIG_DFL;
  sigaction(SIGUSR1, &handlers, NULL);
  sigaction(SIGUSR2, &handlers, NULL);

  pthread_mutex_destroy(&(this->list_lock));
}

bool same_thread(pthread_t& listEntry)
{
  return (listEntry == pthread_self());
}

int ConditionVariable::cond_var_wait(pthread_mutex_t* mutex)
{
  // might want to have checking to ensure that mutex has been acquired and locked
  // put to sleep using pause and then reacquire lock before returning

  // do you want to add to sleeping array before or after releasing lock
  // do you want to remove from sleeping array before or after releasing lock - BEFORE
  // The above two should not matter since mutex does not relate to any globals in this class
  // might want to prioritize making lock free first to allow more concurrency

  // might want to use sigwaitinfo and sigpromask to deal with signal scheduling

  // 1 - unlock mutex
  pthread_mutex_unlock(mutex);

  // 2 - add tid to sleeping list
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.push_front(pthread_self());
  pthread_mutex_unlock(&(this->list_lock));

  // 3 - wait until the signal arrives (blocking the signal earlier helps to ) 
  // sigwait/sigwaitinfo for suspensding execution of THREAD - returns signal number
  // sigprocmask to block/unblock singals for THREAD

  // 4 - thread awoken, remove from sleeping list
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.remove_if(same_thread);
  pthread_mutex_unlock(&(this->list_lock));

  // 5 - reacquire lock
  pthread_mutex_lock(mutex);
  return 0;
}

// // block all signals except SIGUSR1
// // do you want to unblock them later on or leave that up to the user of these functions
// sigset_t full, usr2;
// sigfillset(&full);
// sigprocmask(SIG_BLOCK, &full, NULL);

// sigemptyset(&usr2);
// sigaddset(&usr2, SIGUSR2);

// sigprocmask(SIG_UNBLOCK, &(this->user_sig), NULL);
// sigprocmask(SIG_UNBLOCK, &usr2, NULL);

// // 3 - put thread to sleep until SIGUSR1 received (ensure no locks have been acquired)
// sigwaitinfo(&(this->user_sig), NULL);

// // sigprocmask(SIG_UNBLOCK, &full, NULL);

int ConditionVariable::cond_var_signal()
{
  // return 0 - no threads sleeping (no signal sent)
  // return 1 - atleast one thread was sleeping (one signal sent)
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

int ConditionVariable::cond_var_broadcast()
{

  // return 1 - threads were sleeping
  // return 0 - no threads sleeping

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