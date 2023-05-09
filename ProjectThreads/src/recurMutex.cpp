#include "../include/recurMutex.h"

using namespace std;

pthread_t currThreadID; // the thread that is currently holding the lock being trying to lock
pthread_mutex_t tid_lock; // lock dealing with @currThreadID

int count; // count for how many times the thread has acquired the lock
pthread_mutex_t count_lock; // lock dealing with count