#include "recurMutex.h"
#include "condVar.h"

using namespace std;

pthread_t arr[2];
pthread_mutex_t l;
pthread_cond_t cond;

ConditionVariable* cv;

int main(int argc, char** argv)
{
  // declarations
  void basicRecurTest();
  void* condSigReceiver(void* vargp);
  void* condSigSender(void* vargp);
  void* sender(void* vargp);
  void* receiver(void* vargp);

  // testing

  // CODE after this has set signal handler for SIGUSR1 AND SIGUSR2 to nothing
  cv = new ConditionVariable();

  pthread_mutex_init(&l, NULL);
  pthread_cond_init(&cond, NULL);

  // pthread_create(arr, NULL, sender, NULL);
  // pthread_create((arr + 1), NULL, receiver, NULL);
  // pthread_join(arr[0], NULL);
  // pthread_join(arr[1], NULL);

  // cv = new ConditionVariable();

  // CONDVAR TESTING
  pthread_create(arr, NULL, condSigSender, NULL);
  pthread_create((arr + 1), NULL, condSigReceiver, NULL);
  pthread_join(arr[0], NULL);
  pthread_join(arr[1], NULL);
  // RECURMUTEX TESTING
  // basicRecurTest();

  // signal handler for SIGUSR1 AND SIGUSR2 back to default
  delete cv;

  cout << "MAIN FINISHEDD" << endl;
  pthread_mutex_destroy(&l);
  pthread_cond_destroy(&cond);
  return 0;
}

// THESE TWO METHODS SHOW THAT COND_WAIT GOES TO DEFAULT SIGHANDLER OF THE INCOMING SIGNAL
void* sender(void* vargp)
{
  sleep(2);
  cout << "PASSING SIGUSR2" << endl;
  pthread_kill(arr[1], SIGUSR2);
  sleep(5);
  cout << "COND SIGNAL" << endl;
  pthread_cond_signal(&cond);
}

void* receiver(void* vargp)
{
  pthread_mutex_lock(&l);
    pthread_cond_wait(&cond, &l);
  pthread_mutex_unlock(&l);

  cout << "RECEIVER WOKEN UP" << endl;
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
  cout << "Receiver going to sleep\n" << endl;
  pthread_mutex_lock(&l);
    cv->cond_var_wait(&l);
  pthread_mutex_unlock(&l);
  cout << "Receiver awoken\n" << endl;
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
  cout << "Thread: " << rm->get_lock_owner() << endl;
  delete rm;
  cout << "BASIC RECURSION TEST: PASSSSSSS" << endl;
}