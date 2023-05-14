#include "../include/recurMutex.h"

using namespace std;

int main(int argc, char** argv)
{
  void basicRecurTest();
  basicRecurTest();
  return 0;
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