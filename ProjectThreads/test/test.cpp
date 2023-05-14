#include <gtest/gtest.h>
#include "../include/recurMutex.h"
#include "../include/condVar.h"

using namespace std;

TEST(TempTest, Test1)
{
  // carry out code
  cout << "Test works" << endl;
}

int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}