#include "../include/condVar.h"

// 1 - pause, 2 - nanosleep, 3 - sched_yield
// might want to check out sigwaitinfo and sigprocmask
// need to research more about blocked and non-blocked signals
// for now consider pause
// you will need a variable to confirm that the condVar for init and the one used for the other methods is the same
// you will need a singal handler for SIGUSR1 [default is to terminate which you don't want to occur]
// you could incorporate a bounded buffer here
// since you will need to keep track of which threads are sleeping on the condition to wake them up
  // this will require additional condition variables and mutexes

#define THREAD_COUNT 10

pthread_cond_t* initCondVar = NULL;
pthread_t sleeping_threads[THREAD_COUNT];

void cond_wait_sleeper(int sigNum)
{
  cout << "Received SIGUSR1" <<  endl;
}

ConditionVariable::ConditionVariable()
{
  signal(SIGUSR1, cond_wait_sleeper);
  pthread_mutex_init(&(this->list_lock), NULL);
}

ConditionVariable::~ConditionVariable()
{
  signal(SIGUSR1, SIG_DFL);
  pthread_mutex_destroy(&(this->list_lock));
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
  pthread_mutex_unlock(mutex);
  pause();
  // before acquiring lock gain remove yourself from the sleeping_threads array'
  pthread_mutex_lock(mutex);
  return 0;
}

int ConditionVariable::cond_var_signal()
{
  // may not make much sense now
  return 0;
}

int ConditionVariable::cond_var_broadcast()
{
  // confirm that sleeping_threads[i] is a valid tid
  auto elemItr = this->sleeping_threads.begin();
  while (elemItr != this->sleeping_threads.end())
  {
    pthread_kill(*elemItr, SIGUSR1);
    elemItr++;
  }

  return 0;
}