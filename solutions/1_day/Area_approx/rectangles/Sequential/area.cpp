#ifdef _OPENMP
#include <omp.h>
#endif

#include <iostream>
#include <string>
#include "gtest/gtest.h"

 double func( double x){
  return 1/(1+x*x);
}


int main(int argc, char** argv){
  /**
   * The number of rectangles from 0 to 1, left endpoint
   * and right endpoint, the length of the base of each
   * rectangle.
   */

  //get the number of rectangles from command line
  int steps = std::stoi(argv[1]);
  double x0{0.0};
  double x1{1.0};
  double base = (x1-x0)/steps;

  double approx{0.0};
  int i;
  for(i = 0; i < steps; i++){
    double x1 = x0 + i*base;
    double x2 = x1 + base; 
    double x_mp = (x1 + x2)/2;
    approx += func(x_mp);
  }
  
  approx*= base;

  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  //delete listeners.Release(listeners.default_result_printer()); 

  EXPECT_NEAR(3.14159265358, 4*approx, 1e-5);
  ::testing::InitGoogleTest(&argc, argv);
  auto result = RUN_ALL_TESTS();
  
  return 0;

  

}
