#include "../include/condVar.h"

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