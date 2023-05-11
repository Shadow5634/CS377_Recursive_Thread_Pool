#include "../include/condVar.h"

// 1 - pause, 2 - nanosleep, 3 - sched_yield
// for now consider pause
// you will need a variable to confirm that the condVar for init and the one used for the other methods is the same
// you will need a singal handler for a user defined signal that does nothing
// you could incorporate a bounded buffer here
// since you will need to keep track of which threads are sleeping on the condition to wake them up
  // this will require additional condition variables and mutexes

int cond_var_init(pthread_cond_t* condVar)
{
  // ASSUME INTIALIZING IT WILL NULL - DEFAULT ATTRIBUTES
  return 0;
}

int cond_var_destroy(pthread_cond_t* condVar)
{
  return 0;
}

int cond_var_wait(pthread_cond_t* condVar, pthread_mutex_t* mutex)
{
  return 0;
}

int cond_var_signal(pthread_cond_t* condVar)
{
  return 0;
}

int cond_var_broadcast(pthread_cond_t* condVar)
{
  return 0;
}