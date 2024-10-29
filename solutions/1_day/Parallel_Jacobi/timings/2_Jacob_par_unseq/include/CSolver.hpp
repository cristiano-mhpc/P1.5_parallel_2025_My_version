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
        CSimple_timer t{"FINAL BOSS"};
        std::for_each(std::execution::par_unseq, M.new_field.begin() + n + 1, M.new_field.end() - n - 1, [&n, &M](double& value) mutable {
          //define the indeces using pointer arithmetic 
          auto index = &value - M.new_field.data();
          //if(index%n != 0 && index%n != n-1 ) value = 0.25 * (M.field[index + n] + M.field[index - n] + M.field[index + 1] + M.field[index - 1]);
          value  = (index%n != 0 && index%n != n-1) ? 0.25 * (M.field[index + n] + M.field[index - n] + M.field[index + 1] + M.field[index - 1]): value;
          });
        M.new_field.swap(M.field);
        step++;
      }

    } // while

  }
};



#endif
