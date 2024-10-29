#include <vector>
#include <iostream>
#include <fstream>
#include "CMesh.hpp"
#include "BoundCond.hpp"
#include "CSolver.hpp"


int main(){
  
  CMesh<double> data(5000, boundary_cond<double>);
  CSolver<double> solver; 

  solver.jacobi(data, 100, 25);

  CSimple_timer::print_timing_results();

}
