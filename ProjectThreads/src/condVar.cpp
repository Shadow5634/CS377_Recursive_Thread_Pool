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

  pthread_mutex_init(&(this->sleeping_list_lock), NULL); // initialize mutex
  pthread_mutex_init(&(this->thread_info_lock), NULL);
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

  pthread_mutex_destroy(&(this->sleeping_list_lock)); // free resources for mutex
  pthread_mutex_destroy(&(this->thread_info_lock));
}

/**
 * returns the number of threads that are currently sleeping
*/
int ConditionVariable::sleepingThreadCount()
{
  pthread_mutex_lock(&(this->sleeping_list_lock));
    int size = this->sleeping_threads.size();
  pthread_mutex_unlock(&(this->sleeping_list_lock));

  return size;
}

/**
 * checks wthether the given tid is one of the sleeping threads
*/
bool ConditionVariable::isSleeping(pthread_t tid)
{
  pthread_mutex_lock(&(this->sleeping_list_lock));
    auto ref = find(this->sleeping_threads.begin(), this->sleeping_threads.end(), tid);
  pthread_mutex_unlock(&(this->sleeping_list_lock));

  return (ref != this->sleeping_threads.end());
}

// auto has_struct = this->thread_sleep_info.find(thread_to_wake);
// struct exists for thread to be woken
// if (has_struct != this->thread_sleep_info.end())

/**
 * sends wake up signal to any one of the sleeping threads (if one present)
 * return 0 if no signal (SIGUSR1) sent since no sleeping
 * return 1 if singal (SIGUSR1) was sent
*/
int ConditionVariable::cond_var_signal()
{
  // =================================================================================
  // =================================================================================
  // ANSWER FOLLOWS:
  // =================================================================================
  // =================================================================================

  // TODO: 
  // 1] May want separate return values for:
  //      -No signal since no sleeping
  //      -No signal if signal already sent
  // 2] Check how you want the protections to play out
  //      -Maybe have only 1 lock?
  //      -Dont have nested locks? - nested locks mean 1 lock suffices

  // ASSUMPTIONS:
  // -no signal sent should be updated correctly by the wait method
  // -can't delete struct in wait since it will affect the behaviour here???

  pthread_mutex_lock(&(this->sleeping_list_lock));

    // No sleeping threads
    if(this->sleeping_threads.empty())
    {
      pthread_mutex_unlock(&(this->sleeping_list_lock));
      return 0;
    }

    // arbitarily choosing the first thread to wake (STACK????)
    pthread_t thread_to_wake = this->sleeping_threads.front();

    pthread_mutex_lock(&(this->thread_info_lock));

      thread_info st = this->thread_sleep_info[thread_to_wake];

      // thread to be woken has not been sent signal yet
      // thus send signal and mark signal has been sent
      if (!st.sent_signal)
      {
        pthread_kill(thread_to_wake, SIGUSR1);
        st.sent_signal = true;
      }

    pthread_mutex_unlock(&(this->thread_info_lock));

  pthread_mutex_unlock(&(this->sleeping_list_lock));

  return 1;
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

  pthread_mutex_lock(&(this->sleeping_list_lock));
    auto elemItr = this->sleeping_threads.begin();

    while (elemItr != this->sleeping_threads.end())
    {
      pthread_kill(*elemItr, SIGUSR1);
      elemItr++;
      res = 1;
    }
  pthread_mutex_unlock(&(this->sleeping_list_lock));

  return res;
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
  // necessary so that after adding tid to sleeping_threads, receiving signal does not 
  // cause normal disposition to execute
  sigprocmask(SIG_BLOCK, &(this->user_sig), NULL);

  // add calling thread's id to sleeping list.
  // opens it up to be woken up from calls to broadcast and signal
  pthread_mutex_lock(&(this->sleeping_list_lock));
    this->sleeping_threads.push_front(pthread_self());

    // adding the struct
    // currently assumes not already present or overrides it
    pthread_mutex_lock(&(this->thread_info_lock));
      thread_info st;
      st.tid = pthread_self();
      st.sent_signal = false;

      this->thread_sleep_info[pthread_self()] = st;
    pthread_mutex_unlock(&(this->thread_info_lock));

  pthread_mutex_unlock(&(this->sleeping_list_lock));

  // unlock mutex
  // other thread that acquires this mutex is guarenteed that this thread is 'sleeping'
  pthread_mutex_unlock(mutex);
  
  // putting thread to sleep/suspending execution until SIGUSR1 received (signal/broadcast)
  sigwaitinfo(&(this->user_sig), NULL);

  // removing calling thread id from sleeping list to note that thread is no longer sleeping
  pthread_mutex_lock(&(this->sleeping_list_lock));
    this->sleeping_threads.remove_if(same_thread);
  pthread_mutex_unlock(&(this->sleeping_list_lock));

  // unblocking signal since after being removed from sleeping list
  // it can't be sent SIGUSR1 through signal/broadcast functions being implemented
  sigprocmask(SIG_UNBLOCK, &(this->user_sig), NULL);

  // reacquire lock
  pthread_mutex_lock(mutex);
}
