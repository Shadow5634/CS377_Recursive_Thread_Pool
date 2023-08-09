#include <gtest/gtest.h>
#include "recurMutex.h"
#include "condVar.h"

using namespace std;

// =================================================================================
// =================================================================================
// 1] Tests for recursive mutex implementation (all private for now):
// =================================================================================
// =================================================================================

/**
 * This function seeks to find the factorial of @num for some safety 
 * (not needed here but to make a test), it must acquire @rlock
 * at the start of the function call and release it at end
*/
int recursiveMutexFact(int num, RecursiveLock* rlock)
{
  rlock->lock();
    if (num <= 1) // 0! = 1
    {
      rlock->unlock();
      return 1;
    }

    // recursive call to get factorial
    // result not returned immediately since must unlock recursive mutex
    int val = num * recursiveMutexFact(num - 1, rlock);
  rlock->unlock();
  return val;
}

/**
 * Tests that no deadlocks occur for recusively computing
 * factorials involving acquiring the recursive lock at the start
 * Tests for edge case of 1 when no recursive made as well
*/
// TEST(RecurMutex, computesFactorial)
// {
//   int recursiveMutexFact(int num, RecursiveLock* rlock);

//   RecursiveLock rlock;
//   int fact1 = recursiveMutexFact(1, &rlock);
//   int count1 = rlock.get_acqui_count();
//   bool owner1 = rlock.is_owner(pthread_self());

//   int fact5 = recursiveMutexFact(5, &rlock);
//   int count5 = rlock.get_acqui_count();
//   bool owner5 = rlock.is_owner(pthread_self());

//   EXPECT_EQ(fact1, 1);
//   EXPECT_EQ(count1, 0);
//   EXPECT_EQ(owner1, false);

//   EXPECT_EQ(fact5, 120);
//   EXPECT_EQ(count5, 0);
//   EXPECT_EQ(owner5, false);
// }

/**
 * Ensures that on object initialization the recursive lock is
 * not acquired by any thread i.e. #acquisions = 0
*/
TEST(RecurMutex_1thread, correctIntializations)
{
  RecursiveLock rlock;

  // lock initially free - not acquired and (thus) can't be unlocked
  EXPECT_EQ(rlock.get_acqui_count(), 0);
  EXPECT_EQ(rlock.is_owner(pthread_self()), false);
  EXPECT_EQ(rlock.unlock(), -1);
}

/**
 * Tests that try lock returns correctly
*/
TEST(RecurMutex_1thread, basicTryLockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.try_lock(), 1);
  EXPECT_EQ(rlock.try_lock(), 0);
  EXPECT_EQ(rlock.try_lock(), 0);
}

/**
 * Tests lock returns correctly
*/
TEST(RecurMutex_1thread, basicLockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.lock(), 1);
  EXPECT_EQ(rlock.lock(), 0);
  EXPECT_EQ(rlock.lock(), 0);
}

/**
 * Tests try lock and unlock work correctly in conjunction 
*/
TEST(RecurMutex_1thread, basicTryLock_UnlockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.unlock(), -1);

  EXPECT_EQ(rlock.try_lock(), 1);
  EXPECT_EQ(rlock.try_lock(), 0);

  EXPECT_EQ(rlock.unlock(), 0);
  EXPECT_EQ(rlock.unlock(), 1);
  EXPECT_EQ(rlock.unlock(), -1);
}

/**
 * Tests lock and unlock work correctly in conjunction
*/
TEST(RecurMutex_1thread, basicLock_UnlockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.unlock(), -1);

  EXPECT_EQ(rlock.lock(), 1);
  EXPECT_EQ(rlock.lock(), 0);

  EXPECT_EQ(rlock.unlock(), 0);
  EXPECT_EQ(rlock.unlock(), 1);
  EXPECT_EQ(rlock.unlock(), -1);
}

/**
 * Tests acqui_count and is_owner work correctly in conjunction with try_lock
*/
TEST(RecurMutex_1thread, correctCountAndOwner_TryLockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.get_acqui_count(), 0);
  EXPECT_EQ(rlock.is_owner(pthread_self()), false);

  EXPECT_EQ(rlock.try_lock(), 1);
    EXPECT_EQ(rlock.get_acqui_count(), 1);
    EXPECT_EQ(rlock.is_owner(pthread_self()), true);
  EXPECT_EQ(rlock.try_lock(), 0);
    EXPECT_EQ(rlock.get_acqui_count(), 2);
    EXPECT_EQ(rlock.is_owner(pthread_self()), true);
}

/**
 * Tests acqui_count and is_owner work correctly in conjunction with lock
*/
TEST(RecurMutex_1thread, correctCountAndOwner_LockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.get_acqui_count(), 0);
  EXPECT_EQ(rlock.is_owner(pthread_self()), false);

  EXPECT_EQ(rlock.lock(), 1);
    EXPECT_EQ(rlock.get_acqui_count(), 1);
    EXPECT_EQ(rlock.is_owner(pthread_self()), true);
  EXPECT_EQ(rlock.lock(), 0);
    EXPECT_EQ(rlock.get_acqui_count(), 2);
    EXPECT_EQ(rlock.is_owner(pthread_self()), true);
}

/**
 * Test that try_lock never fails after a call to lock
*/
TEST(RecurMutex_1thread, TryLock_LockTest)
{
  RecursiveLock rlock;

  EXPECT_EQ(rlock.lock(), 1);
  EXPECT_EQ(rlock.try_lock(), 0);

  RecursiveLock rlock2;

  EXPECT_EQ(rlock2.lock(), 1);
  EXPECT_EQ(rlock2.lock(), 0);
  EXPECT_EQ(rlock.try_lock(), 0);
}

/**
 * checks that all functions work correctly in conjunction
 * Note that this leaves out try_lock
*/
TEST(RecurMutex_1thread, allFunctionsTest)
{
  RecursiveLock rlock;
  int extra_aqui = 4;

  EXPECT_EQ(rlock.lock(), 1);

  for (int i = 0; i < extra_aqui; i++)
  {
    EXPECT_EQ(rlock.lock(), 0);

    // checks if count and owner updated correctly
    EXPECT_EQ(rlock.is_owner(pthread_self()), true);
    EXPECT_EQ(rlock.get_acqui_count(), i + 2);
  }

  // only partially unlocking the mutex
  for (int i = 0; i < extra_aqui; i++)
  {
    // mutex should be unlocked only by 1 layer
    EXPECT_EQ(rlock.unlock(), 0);

    // owner remains the same and count updated correctly
    EXPECT_EQ(rlock.is_owner(pthread_self()), true);
    EXPECT_EQ(rlock.get_acqui_count(), extra_aqui - i);
  }

  // mutex fully unlocked
  EXPECT_EQ(rlock.unlock(), 1);
  EXPECT_EQ(rlock.get_acqui_count(), 0);
  EXPECT_EQ(rlock.is_owner(pthread_self()), false);
}

/**
 * Checks that try lock returns -1 when trying to acquire a lock
 * that has already been acquired by a different thread
*/
// TEST(RecurMutex, basicTryLockTest)
// {
//   void* tryLockHelper(void* vargp);

//   RecursiveLock rlock;
//   rlock.lock();

//   pthread_t fail;
//   pthread_create(&fail, NULL, tryLockHelper, (void*) &rlock);
//   pthread_join(fail, NULL);

//   rlock.unlock();
// }

/**
 * a thread function that tries to lock a recursive lock that
 * is owned by some other thread. Thus try_lock should return -1
*/
void* tryLockHelper(void* vargp)
{
  RecursiveLock* rlock = (RecursiveLock*) vargp;
  EXPECT_EQ(rlock->try_lock(), -1);
  return NULL;
}

// =================================================================================
// =================================================================================
// 2] Tests for conditional variables implementation (all private for now):
// =================================================================================
// =================================================================================

/**
 * This strcture is used to wrap a ConditionVariable object
 * and the mutex associated with its cond_wait in order to 
 * pass it into a thread function
 * modify val is used for checks
*/
typedef struct encompass 
{
  ConditionVariable* condVar;
  pthread_mutex_t* mutex;
  int modify;
} encompass;

/**
 * calls cond_var_wait while ensuring that the associated
 * mutex is locked before and unlocked after the wait
*/
void* condHelper(void* vargp)
{
  encompass* condStruct = (encompass*) vargp;
  ConditionVariable* condVar = condStruct->condVar;
  pthread_mutex_t* mutex = condStruct->mutex;

  pthread_mutex_lock(mutex);
    condVar->cond_var_wait(mutex);
  pthread_mutex_unlock(mutex);

  condStruct->modify = 10;
  return NULL;
}

/**
 * Ensures that at start no threads are sleeping on the condition
 * thus must also be the case that signal and broadcast do not send any signals
*/
// TEST(CondVar, correctIntializations)
// {
//   ConditionVariable* condVar = new ConditionVariable();

//   EXPECT_EQ(condVar->sleepingThreadCount(), 0);
//   EXPECT_EQ(condVar->isSleeping(pthread_self()), false);

//   EXPECT_EQ(condVar->cond_var_signal(), 0);
//   EXPECT_EQ(condVar->cond_var_broadcast(), 0);

//   delete condVar;
// }

/**
 * Checks whether wait seems to function properly - indeed puts thread to sleep
 * and which is awoken peacefully when sent SIGUSR1 through pthread_kill
 * signal/broadcast not used to limit dependancy on other methods
 * thus must also be the case that signal and broadcast do not send any signals
*/
// TEST(CondVar, basicWaitTest)
// {
//   void* condHelper(void* vargp);

//   ConditionVariable* condVar = new ConditionVariable();
//   pthread_mutex_t mutex;
//   pthread_mutex_init(&mutex, NULL);

//   encompass condStruct;
//   condStruct.condVar = condVar;
//   condStruct.mutex = &mutex;
//   condStruct.modify = 0;

//   pthread_t thread;
//   pthread_create(&thread, NULL, condHelper, (void*) &condStruct);
//   usleep(1); // ensure that created thread goes to sleep
//   pthread_kill(thread, SIGUSR1);
//   usleep(1); // ensure that signal wakes up created thread
//   EXPECT_EQ(condStruct.modify, 10);

//   pthread_join(thread, NULL);
//   pthread_mutex_destroy(&mutex);

//   delete condVar;
// }

/**
 * Confirms that cond_var_broadcast wakes up all the threads waiting on the condition
*/
// TEST(CondVar, broadcastTest)
// {
//   ConditionVariable* condVar = new ConditionVariable();
//   pthread_mutex_t mutex;
//   pthread_mutex_init(&mutex, NULL);
//   pthread_t tid_arr[2];

//   encompass condStruct;
//   condStruct.condVar = condVar;
//   condStruct.mutex = &mutex;

//   for (int i = 0; i < 2; i++)
//   {
//     pthread_create((tid_arr + i), NULL, condHelper, (void*) &condStruct);
//   }

//   usleep(1); // letting threads be put to sleep
//   int val = condVar->cond_var_broadcast();
//   usleep(1); // letting threads wake up
//   EXPECT_EQ(val, 1);  // signals were sent
//   EXPECT_EQ(condVar->sleepingThreadCount(), 0);  // no sleeping threads

//   for (int i = 0; i < 2; i++)
//   {
//     pthread_join(tid_arr[i], NULL);
//   }

//   pthread_mutex_destroy(&mutex);
//   delete condVar;
// }

/**
 * Checks to see that cond_var_signal wakes up only 1 thread at a time
*/
// TEST(CondVar, signalTest)
// {
//   ConditionVariable* condVar = new ConditionVariable();
//   pthread_mutex_t mutex;
//   pthread_mutex_init(&mutex, NULL);
//   pthread_t tid_arr[2];

//   encompass condStruct;
//   condStruct.condVar = condVar;
//   condStruct.mutex = &mutex;

//   for (int i = 0; i < 2; i++)
//   {
//     pthread_create((tid_arr + i), NULL, condHelper, (void*) &condStruct);
//   }

//   usleep(1); // letting threads be put to sleep
//   int val = condVar->cond_var_signal();
//   usleep(1); // letting a thread wake up
//   EXPECT_EQ(val, 1); // signal sent
//   EXPECT_EQ(condVar->sleepingThreadCount(), 1); // only 1 of 2 threads awoken

//   int val2 = condVar->cond_var_signal();
//   usleep(1); // letting a thread wake up
//   EXPECT_EQ(val2, 1); // signal sent
//   EXPECT_EQ(condVar->sleepingThreadCount(), 0); // 1 of 1 thread awoken

//   for (int i = 0; i < 2; i++)
//   {
//     pthread_join(tid_arr[i], NULL);
//   }

//   pthread_mutex_destroy(&mutex);
//   delete condVar;
// }

// =================================================================================
// =================================================================================
// 3] Main method to execute all the above tests
// =================================================================================
// =================================================================================

int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}