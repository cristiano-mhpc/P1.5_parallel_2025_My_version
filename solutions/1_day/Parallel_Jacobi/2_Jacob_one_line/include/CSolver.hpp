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

      #ifdef CASE0
      {
        CSimple_timer t{"Not_parallel"};
        for (i = 1; i < n - 1; i++) {
          for (j = 1; j < n - 1; j++) {
           M.new_field[i * n + j] =
               0.25 * (M.field[(i + 1) * n + j] + M.field[(i - 1) * n + j] +
                      M.field[i * n + j + 1] + M.field[i * n + j - 1]);
          }
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

      #endif

      #ifdef  CASE1
      {
        std::vector<int> index(n-2);
        for (int j = 0; j < n-2; j++) {
            index[j] = j + 1;
        }
        CSimple_timer t{"Inner loop parallel"};
        for (i = 1; i < n - 1; i++) {
          //parallelize along the columns
          std::for_each(std::execution::par, index.begin(), index.end(),[&M, &i, &n](int j){
                M.new_field[i * n + j] = 0.25 * (M.field[(i + 1) * n + j] + M.field[(i - 1) * n + j] +
                      M.field[i * n + j + 1] + M.field[i * n + j - 1]);
          });
        }
      }

      //if step == PrintInterval, print new_field
      if(step%PrintInterval == 0){
        std::ostringstream temp;
        temp << "./data_1/" << std::setw(5) << std::setfill('0') << step <<".dat";
        std::ofstream filevar; 
 
        //std::string filename = "./data/"+name.str()+".dat"; 
        filevar.open(temp.str());
        M.print(filevar, M.new_field, M.N);
      }

      #endif

      #ifdef  CASE2
      std::vector<size_t> index;
      for (i = 1; i < n - 1; i++) {
        for (j = 1; j < n - 1; j++) {
           index.push_back((i*n) + j);
        }
       }
      {
          CSimple_timer t{"Single Loop"};
          std::for_each(std::execution::par, index.begin(), index.end(),[&M, &n](int k){
          M.new_field[k] = 0.25*(M.field[k + n] + M.field[k - n] + M.field[k + 1] + M.field[k - 1]);
          });
      }

      //if step == PrintInterval, print new_field
      if(step%PrintInterval == 0){
        std::ostringstream temp;
        temp << "./data_2/" << std::setw(5) << std::setfill('0') << step <<".dat";
        std::ofstream filevar; 
 
        //std::string filename = "./data/"+name.str()+".dat"; 
        filevar.open(temp.str());
        M.print(filevar, M.new_field, M.N);
      }

      #endif

      #ifdef  CASE3
      {
          CSimple_timer t{"FINAL BOSS"};
          std::for_each(std::execution::par, M.new_field.begin(), M.new_field.end(),[k = n, &n, &M](double value) mutable {
            value = 0.25*(M.field[k + n] + M.field[k - n] + M.field[k + 1] + M.field[k - 1]);
            k++;
          });
      }
      
      //if step == PrintInterval, print new_field
           if(step%PrintInterval == 0){
           std::ostringstream temp;
           temp << "./data_3/" << std::setw(5) << std::setfill('0') << step <<".dat";
           std::ofstream filevar; 
          //std::string filename = "./data/"+name.str()+".dat"; 
           filevar.open(temp.str());
           M.print(filevar, M.new_field, M.N);
      }
      
      #endif

      M.new_field.swap(M.field);

      step++;

    } // while
  }
};

#endif
