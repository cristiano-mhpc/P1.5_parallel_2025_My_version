#include <vector>
#include <iostream>
#include <fstream>
#include "CMesh.hpp"
#include "BoundCond.hpp"
#include "CSolver.hpp"


int main(int argc, char** argv){


    int size = std::stoi(argv[1]);
    CMesh<double> data(size, boundary_cond<double>);
    CSolver<double> solver; 
    solver.jacobi(data, size, size);
  
    CSimple_timer::print_timing_results(size);

}
