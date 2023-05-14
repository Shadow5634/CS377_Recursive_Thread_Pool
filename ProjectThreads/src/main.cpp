#include "../include/recurMutex.h"
#include "../include/condVar.h"

using namespace std;

pthread_t arr[2];
pthread_mutex_t l;

ConditionVariable* cv;

int main(int argc, char** argv)
{
  // declarations
  void basicRecurTest();
  void* condSigReceiver(void* vargp);
  void* condSigSender(void* vargp);

  // testing

  pthread_mutex_init(&l, NULL);

  cv = new ConditionVariable();

  pthread_create(arr, NULL, condSigSender, NULL);
  pthread_create((arr + 1), NULL, condSigReceiver, NULL);
  pthread_join(arr[0], NULL);
  pthread_join(arr[1], NULL);

  // basicRecurTest();

  delete cv;
  return 0;
}

void* condSigSender(void* vargp)
{
  cout << "Sender starting\n" << endl;
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