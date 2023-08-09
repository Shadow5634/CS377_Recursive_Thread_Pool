#include "recurMutex.h"
#include "condVar.h"

using namespace std;

pthread_t arr[2];
pthread_mutex_t l;
pthread_cond_t cond;

// ### MY IMPLEMENTATION START ###
  void cond_wait_usr_sleeper(int sigNum)
  {
    // struct sigaction handlers;
    // handlers.sa_handler = cond_wait_usr_sleeper;
    // sigaction(SIGUSR1, &handlers, NULL);
    // sigaction(SIGUSR2, &handlers, NULL);

    // struct sigaction handlers;
    // handlers.sa_handler = SIG_DFL;
    // sigaction(SIGUSR1, &handlers, NULL);
    // sigaction(SIGUSR2, &handlers, NULL);
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

ConditionVariable* cv;

void* condHelper(void* vargp)
{
  cout << "going to sleep" << endl;
  pthread_mutex_lock(&l);
    pthread_cond_wait(&cond, &l);
  pthread_mutex_unlock(&l);
  cout << "woken up" << endl;

  return NULL;
}

int main(int argc, char** argv)
{
  // declarations
  void basicRecurTest();
  void* condSigReceiver(void* vargp);
  void* condSigSender(void* vargp);
  void* condSigReceiverACT(void* vargp);
  void* condSigSenderACT(void* vargp);
  void* sender(void* vargp);
  void* receiver(void* vargp);

  cout << "End of main" << endl;

  return 0;
}

// testing

// CODE after this has set signal handler for SIGUSR1 AND SIGUSR2 to nothing
// cv = new ConditionVariable();

// pthread_mutex_init(&l, NULL);
// pthread_cond_init(&cond, NULL);

// pthread_t t;
// pthread_create(&t, NULL, condHelper, NULL);
// cout << "Sending signal" << endl;
// pthread_cond_signal(&cond);
// pthread_join(t, NULL);

// pthread_create(arr, NULL, sender, NULL);
// pthread_create((arr + 1), NULL, receiver, NULL);
// pthread_join(arr[0], NULL);
// pthread_join(arr[1], NULL);

// cv = new ConditionVariable();

// pthread_create(arr, NULL, condSigSenderACT, NULL);
// pthread_create((arr + 1), NULL, condSigReceiverACT, NULL);
// pthread_join(arr[0], NULL);
// pthread_join(arr[1], NULL);

// CONDVAR TESTING
// pthread_create(arr, NULL, condSigSender, NULL);
// pthread_create((arr + 1), NULL, condSigReceiver, NULL);
// pthread_join(arr[0], NULL);
// pthread_join(arr[1], NULL);
// RECURMUTEX TESTING
// basicRecurTest();

// signal handler for SIGUSR1 AND SIGUSR2 back to default
// delete cv;
// // pthread_kill(1, SIGUSR1);

// cout << "MAIN FINISHEDD" << endl;
// pthread_mutex_destroy(&l);
// pthread_cond_destroy(&cond);

// THESE TWO METHODS SHOW THAT COND_WAIT GOES TO DEFAULT SIGHANDLER OF THE INCOMING SIGNAL
void* sender(void* vargp)
{
  sleep(2);
  cout << "PASSING SIGUSR2" << endl;
  pthread_kill(arr[1], SIGUSR2);
  sleep(5);
  cout << "COND SIGNAL" << endl;
  pthread_cond_signal(&cond);

  return NULL;
}

void* receiver(void* vargp)
{
  pthread_mutex_lock(&l);
    pthread_cond_wait(&cond, &l);
  pthread_mutex_unlock(&l);

  cout << "RECEIVER WOKEN UP" << endl;
  return NULL;
}

void* condSigSenderACT(void* vargp)
{
  cout << "Sender sending broadcast\n" << endl;
  cv->cond_var_broadcast();
  return NULL;
}

void* condSigReceiverACT(void* vargp)
{
  cout << "Receiver cond waiting\n" << endl;
  pthread_mutex_lock(&l);
    cv->cond_var_wait(&l);
  pthread_mutex_unlock(&l);
  cout << "Receiver awoken from cond waiting\n" << endl;
  return NULL;
}

void* condSigSender(void* vargp)
{
  cout << "Sender starting\n" << endl;
  // cout << "Sender sending broadcast\n" << endl;
  // cv->cond_var_broadcast();
  // cout << "sending random signal usr2" << endl;
  // pthread_kill(arr[1], SIGUSR2);

  sleep(5);
  cout << "Sender sending broadcast\n" << endl;
  cv->cond_var_broadcast();
  return NULL;
}

void* condSigReceiver(void* vargp)
{
  cout << "Receiver cond waiting\n" << endl;
  pthread_mutex_lock(&l);
    cv->cond_var_wait(&l);
  pthread_mutex_unlock(&l);
  cout << "Receiver awoken from cond\n" << endl;
  return NULL;
}

void basicRecurTest()
{
  RecursiveLock* rm = new RecursiveLock();

  for (int i = 1; i <= 5; i++)
  {
    rm->recur_mutex_lock();
  }

  for (int i = 1; i <= 5; i++)
  {
    rm->recur_mutex_unlock();
  }

  cout << "Count: " << rm->get_acqui_count() << endl;
  // cout << "Thread: " << rm->get_lock_owner() << endl;
  delete rm;
  cout << "BASIC RECURSION TEST: PASSSSSSS" << endl;
}