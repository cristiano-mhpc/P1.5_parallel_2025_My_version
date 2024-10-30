#ifndef CSolver_HPP
#define CSolver_HPP

#include <sstream>
#include "CMesh.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <mpi.h>

template <typename T>
class CSolver {
public:
  void jacobi(CMesh<T> &M, int my_rank, int comm_sz, const size_t &max_steps,
              const size_t &PrintInterval, MPI_Comm comm) {

    size_t step{0};
    size_t i, j;
    size_t n = M.rows;
    size_t m = M.col;

    while (step < max_steps) {
      /**
       * each process performs TWO MPI_Send_recv()s one with the PREVIOUS process
       * and another with the NEXT process.
      */
      int next_rank = (my_rank == comm_sz - 1) ? MPI_PROC_NULL : my_rank + 1;
      int prev_rank = (rank == 0) ? MPI_PROC_NULL : rank - 1;
      
      //send and receive with the prev_rank
      MPI_Send_recv(&M.field[m + 1], m-2, MPI_DOUBLE, prev_rank, 0,
                   &M.field[1], m-2, MPI_DOUBLE, prev_rank, 0, comm, MPI_STATUS_IGNORE);

      //send and receive with the next_rank
      MPI_Send_recv(&M.field[m*(m - 1) + 1], m-2, MPI_DOUBLE, next_rank, 1,
                   &M.field[m*m + 1], m-2, MPI_DOUBLE, next_rank, 1, comm, MPI_STATUS_IGNORE);

      for (i = 1; i < n - 1 ; i++) {
        for (j = 1; j < m - 1; j++) {
          M.new_field[i * m + j] =
              0.25*(M.field[(i + 1) * m + j] + M.field[(i - 1) * m + j] +
                      M.field[i * m + j + 1] + M.field[i * m + j - 1]);
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
