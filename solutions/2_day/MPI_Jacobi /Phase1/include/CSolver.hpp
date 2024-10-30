#ifndef CSolver_HPP
#define CSolver_HPP

#include <sstream>
#include "CMesh.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

template <typename T>
class CSolver {
public:
  void jacobi(CMesh<T> &M, const size_t &max_steps,
              const size_t &PrintInterval) {

    size_t step{0};
    size_t i, j;
    size_t n = M.rows;
    size_t m = M.col;

    while (step < max_steps) {
      for (i = 1; i < rows - 1; i++) {
        for (j = 1; j < colm - 1; j++) {
          M.new_field[i * n + j] =
              0.25 * (M.field[(i + 1) * n + j] + M.field[(i - 1) * n + j] +
                      M.field[i * n + j + 1] + M.field[i * n + j - 1]);
        }
      }

      //if step == PrintInterval, print new_field
      if(step%PrintInterval == 0){
        std::ostringstream temp;
        temp << "./data/" << std::setw(5) << std::setfill('0') << step <<".dat";
        std::ofstream filevar; 
 
        //std::string filename = "./data/"+name.str()+".dat"; 
        filevar.open(temp.str());
        M.print(filevar, M.new_field, M.N);
      }

      M.new_field.swap(M.field);

      step++;

    } // while
  }
};

#endif
