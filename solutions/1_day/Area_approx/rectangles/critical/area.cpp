
#include <omp.h>
#include <iostream>
#include "CSimple_timer.hpp"
#include <string>
#include "gtest/gtest.h"

double func( double x){
  return 1/(1+x*x);
}

double reduction(double x0, double x1, int steps, double(*func)( double)){
  double approx{0.0};
  double base = (x1-x0)/steps;
  int i;
  {  
    CSimple_timer t{"omp"};
  # pragma omp parallel for private(i) reduction(+:approx) 
    for(i = 0; i < steps; i++){
      double x1 = x0 + i*base;
      double x2 = x1 + base; 
  # pragma omp critical
      approx += func((x1 + x2)/2);
    }
  }
  approx*= base;

  return approx;

}


int main(int argc, char** argv){
  /**
   * The number of rectangles from 0 to 1, left endpoint
   * and right endpoint, the length of the base of each
   * rectangle.
   */

  //get the number of rectangles from command line

  if(argc != 2){
    std::cout << "Use: pass the number of rectangles and the number of threads in the command line." << std::endl;
    return 1;
  }

  int steps = std::stoi(argv[1]);
  double x0{0.0};
  double x1{1.0};
  //double base = (x1-x0)/steps;

  
  /*
  double approx{0.0};
  int i;
  {  
    CSimple_timer t{"omp"};
  # pragma omp parallel for private(i) reduction(+:approx) 
    for(i = 0; i < steps; i++){
      double x1 = x0 + i*base;
      double x2 = x1 + base; 

      approx += func((x1 + x2)/2);
    }
  }
  
  approx*= base;
  */

  double approx = reduction(x0, x1, steps, func);

  CSimple_timer::print_timing_results();
  
  return 0;

}
