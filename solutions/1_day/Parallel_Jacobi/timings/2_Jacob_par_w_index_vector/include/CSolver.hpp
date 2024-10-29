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
    size_t i,j,k;
    size_t n = M.N;


    /*
    {
      //Time the creation and polulation of index vector
      CSimple_timer t{"Creating the vector"};
      std::vector<size_t> index;
      size_t i, j;
      for (i = 1; i < n - 1; i++) {
          for (j = 1; j < n - 1; j++) {
            index.push_back((i*n) + j);
          }
       }
    }
*/
    //call them again as they are out of scope
    std::vector<size_t> index;
    for (i = 1; i < n - 1; i++) {
        for (j = 1; j < n - 1; j++) {
            index.push_back((i*n) + j);
        }
    }

    
    while (step < max_steps) {
    {
      CSimple_timer t{"With_vector_index"};
      {
        std::for_each(std::execution::par, index.begin(), index.end(),[&M, &n](int k){
          M.new_field[k] = 0.25*(M.field[k + n] + M.field[k - n] + M.field[k + 1] + M.field[k - 1]);
        });
      

      M.new_field.swap(M.field);
      step++;
      }
    } 

    }// while


  }

};//Csolver

#endif
