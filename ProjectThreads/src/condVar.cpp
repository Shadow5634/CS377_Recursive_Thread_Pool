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
  // do nothing
}

int cond_var_init(pthread_cond_t* condVar)
{
  // ASSUME INTIALIZING IT WILL NULL - DEFAULT ATTRIBUTES
  // create cond var, store address, and use a user signal for waiting simulation
  pthread_cond_init(condVar, NULL);
  initCondVar = condVar;
  signal(SIGUSR1, cond_wait_sleeper);
  return 0;
}

int cond_var_destroy(pthread_cond_t* condVar)
{
  // destroy cond var, remove address, and get rid of signal handler 
  pthread_cond_destroy(condVar);
  initCondVar = NULL;
  signal(SIGUSR1, SIG_DFL);
  return 0;
}

int cond_var_wait(pthread_cond_t* condVar, pthread_mutex_t* mutex)
{
  // might want to have checking to ensure that mutex has been acquired and locked
  // put to sleep using pause and then reacquire lock before returning
  // do you want to add to sleeping array before or after releasing lock
  // do you want to remove from sleeping array before or after releasing lock
  pthread_mutex_unlock(mutex);
  pause();
  // before acquiring lock gain remove yourself from the sleeping_threads array'
  // might want to use sigwaitinfo and sigpromask to deal with signal scheduling
  pthread_mutex_lock(mutex);
  return 0;
}

int cond_var_signal(pthread_cond_t* condVar)
{
  // may not make much sense now
  return 0;
}

int cond_var_broadcast(pthread_cond_t* condVar)
{
  // confirm that sleeping_threads[i] is a valid tid
  for (int i = 0; i < THREAD_COUNT; i++)
  {
    pthread_kill(sleeping_threads[i], SIGUSR1);
  }

  return 0;
}