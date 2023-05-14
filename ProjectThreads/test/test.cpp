#include <gtest/gtest.h>

using namespace std;

TEST(TempTest, Test1)
{
  // carry out code
  cout << "TEST RUNS" << endl;
}

int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}