#ifndef CSolver_HPP
#define CSolver_HPP

#include <sstream>
#include "CMesh.hpp"
#include "CSimple_timer.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <execution>
#include <algorithm>

template <typename T>
class CSolver {
public:
  void jacobi(CMesh<T> &M, const size_t &max_steps,
              const size_t &PrintInterval) {
    size_t step{0};
    size_t i, j, k;
    size_t n = M.N;
    
    while (step < max_steps) {
     { 
      CSimple_timer t{"Not_parallel"};
      for (i = 1; i < n - 1; i++) {
        for (j = 1; j < n - 1; j++) {
          M.new_field[i * n + j] =
               0.25 * (M.field[(i + 1) * n + j] + M.field[(i - 1) * n + j] +
                      M.field[i * n + j + 1] + M.field[i * n + j - 1]);
        }
      }
      M.new_field.swap(M.field);
      step++;
      
     }
    } // while 
  }
};



#endif
