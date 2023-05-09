#include "../include/recurMutex.h"

using namespace std;

pthread_t currThreadID; // the thread that is currently holding the lock being trying to lock
pid_t proc_id; // the process that the thread belongs to
pthread_mutex_t id_lock; // lock dealing with @currThreadID and @proc_id

int count; // count for how many times the thread has acquired the lock
pthread_mutex_t count_lock; // lock dealing with count

// makes a recursive lock that shall be used by recur_mutex_lock and recur_mutex_unlock (as parameters)
int recur_mutex_init(pthread_mutex_t* mutex);

// destroys the created recursive lock
int recur_mutex_destroy(pthread_mutex_t* mutex);

// tries to lock a mutex using recursive lock approach
int recur_mutex_lock(pthread_mutex_t* mutex);

// tries to unlock a mutex using recursive lock approach
int recur_mutex_lock(pthread_mutex_t* mutex);