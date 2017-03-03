/*************************************************************************
* Copyright (c) 2017 François Trudel
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*************************************************************************/
#include "gtest/gtest.h"

TEST(blahTest, blah1) {
  EXPECT_EQ(1, 1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  int returnValue;

  //Do whatever setup here you will need for your tests here
  //
  //

  returnValue = RUN_ALL_TESTS();

  std::cout << "WELL DONE! THAT WAS SO GOOD I THOUGHT YOU WERE POO" << std::endl;

  //Do Your teardown here if required
  //
  //

  return returnValue;
}