#include <gtest/gtest.h>
#include "recurMutex.h"
#include "recurMutex.h"

using namespace std;

// =================================================================================
// =================================================================================
// 1] Tests for recursive mutex implementation (all private for now):
// =================================================================================
// =================================================================================

int recursiveMutexFact(int num, RecursiveLock* rlock)
{
  rlock->recur_mutex_lock();
    if (num <= 1) // 0! = 1
    {
      rlock->recur_mutex_unlock();
      return 1;
    }

    int val = num * recursiveMutexFact(num - 1, rlock);
  rlock->recur_mutex_unlock();
  return val;
}

TEST(RecurMutex, correctIntializations)
{
  RecursiveLock* rlock = new RecursiveLock();

  // confirms that lock is free initially
  EXPECT_EQ(rlock->get_acqui_count(), 0);
  EXPECT_EQ(rlock->get_lock_owner(), 0);

  delete rlock;
}

TEST(RecurMutex, basicUnlockTest)
{
  RecursiveLock* rlock = new RecursiveLock();
  EXPECT_EQ(rlock->recur_mutex_unlock(), -1);
  delete rlock;
}

TEST(RecurMutex, computesBasicFactorial)
{
  int recursiveMutexFact(int num, RecursiveLock* rlock);

  RecursiveLock* rlock = new RecursiveLock();
  int fact1 = recursiveMutexFact(1, rlock);
  int count1 = rlock->get_acqui_count();
  int owner1 = rlock->get_lock_owner();

  int fact5 = recursiveMutexFact(5, rlock);
  int count5 = rlock->get_acqui_count();
  int owner5 = rlock->get_lock_owner();

  delete rlock;

  EXPECT_EQ(fact1, 1);
  EXPECT_EQ(count1, 0);
  EXPECT_EQ(owner1, 0);

  EXPECT_EQ(fact5, 120);
  EXPECT_EQ(count5, 0);
  EXPECT_EQ(owner5, 0);
}

TEST(RecurMutex, correctCountAndOwnerForLock)
{
  RecursiveLock* rlock = new RecursiveLock();
  int aq = 3;

  for (int i = 0; i < aq; i++)
  {
    int val = rlock->recur_mutex_lock();

    // checks if lock returns the correct value
    if (i == 0)
    {
      EXPECT_EQ(val, 1);
      cout << "correct starting lock value" << endl;
    }
    else
    {
      EXPECT_EQ(val, 0);
    }

    // checks if count and owner updated correctly
    EXPECT_EQ(rlock->get_lock_owner(), pthread_self());
    EXPECT_EQ(rlock->get_acqui_count(), i + 1);
  }

  for (int i = 0; i < 2; i++)
  {
    // mutex should be unlocked only by 1 layer
    EXPECT_EQ(rlock->recur_mutex_unlock(), 0);

    // owner remains the same and count updated correctly
    EXPECT_EQ(rlock->get_lock_owner(), pthread_self());
    EXPECT_EQ(rlock->get_acqui_count(), aq - i - 1);
  }

  // mutex fully unlocked
  EXPECT_EQ(rlock->recur_mutex_unlock(), 1);
  EXPECT_EQ(rlock->get_acqui_count(), 0);
  EXPECT_EQ(rlock->get_lock_owner(), 0);

  delete rlock;
}

// =================================================================================
// =================================================================================
// 2] Tests for conditional variables implementation (all private for now):
// =================================================================================
// =================================================================================

TEST(CondVar, Test1)
{
  // carry out code
  cout << "Test suite 2, test1 works" << endl;
}

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