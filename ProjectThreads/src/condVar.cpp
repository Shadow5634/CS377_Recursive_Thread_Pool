#include "../include/condVar.h"
// #include <condvar.h>

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

void cond_wait_sleeper(int sigNum)
{
  cout << "Received SIGUSR1" <<  endl;
}

ConditionVariable::ConditionVariable()
{
  sigemptyset(&(this->user_sig));
  sigaddset(&(this->user_sig), SIGUSR1);
  signal(SIGUSR1, cond_wait_sleeper);
  pthread_mutex_init(&(this->list_lock), NULL);
}

ConditionVariable::~ConditionVariable()
{
  signal(SIGUSR1, SIG_DFL);
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

  // block all signals except SIGUSR1
  // do you want to unblock them later on or leave that up to the user of these functions
  sigset_t full;
  sigfillset(&full);
  sigprocmask(SIG_BLOCK, &full, NULL);
  sigprocmask(SIG_UNBLOCK, &(this->user_sig), NULL);

  // 3 - put thread to sleep until SIGUSR1 received (ensure no locks have been acquired)
  sigwaitinfo(&(this->user_sig), NULL);

  // sigprocmask(SIG_UNBLOCK, &full, NULL);

  // 4 - thread awoken, remove from sleeping list
  pthread_mutex_lock(&(this->list_lock));
    this->sleeping_threads.remove_if(same_thread);
  pthread_mutex_unlock(&(this->list_lock));

  // 5 - reacquire lock
  pthread_mutex_lock(mutex);
  return 0;
}

int ConditionVariable::cond_var_signal()
{
  // how do you ensure that a random #threads >= 1 could be woken up
  return 0;
}

int ConditionVariable::cond_var_broadcast()
{

  // return 1 - threads were sleeping
  // return 0 - no threads sleeping

  int res = 0;

  // list insertion/deletion ensures tid are all valid
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