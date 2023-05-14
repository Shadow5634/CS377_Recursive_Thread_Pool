#include "../include/recurMutex.h"

using namespace std;

int arr[2];

int main(int argc, char** argv)
{
  // declarations
  void basicRecurTest();
  void condSigReceiver();
  void condSigSender();

  // testing

  // pthread_create();

  // basicRecurTest();
  return 0;
}

void* condSigSender(void* vargp)
{
  return NULL;
}

void* condSigReceiver(void* vargp)
{
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