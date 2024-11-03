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
              const size_t &PrintInterval, size_t local_rows, int rem, MPI_Comm comm){
    size_t step{0};
    size_t i, j;
    size_t n = M.rows;
    size_t m = M.colm;

    while (step < max_steps) {
      /**
       * each process performs TWO MPI_Send_recv()s one with the PREVIOUS process
       * and another with the NEXT process.
      */

      for (i = 2; i < n - 2 ; i++) {
        for (j = 1; j < m - 1; j++) {
          M.new_field[i * m + j] =
              0.25*(M.field[(i + 1) * m + j] + M.field[(i - 1) * m + j] +
                      M.field[i * m + j + 1] + M.field[i * m + j - 1]);
        }
      }

      int next_rank = (my_rank == comm_sz - 1) ? MPI_PROC_NULL : my_rank + 1;
      int prev_rank = (my_rank == 0) ? MPI_PROC_NULL : my_rank - 1;
      
      MPI_Request request_send, request_recv, request_send2, request_recv2;

      //send to prev_rank, recv from next_rank
      MPI_Isend(&M.field[m + 1], m-2, MPI_DOUBLE, prev_rank, 1, comm, &request_send);
      MPI_Irecv(&M.field[(n-1)*m + 1], m - 2, MPI_DOUBLE, next_rank, 1, comm, &request_recv);


      //send to next_rank, recv from prev_rank
      MPI_Isend(&M.field[(n-2)*(m) + 1], m-2, MPI_DOUBLE, next_rank, 0, comm, &request_send2);
      MPI_Irecv(&M.field[1], m - 2, MPI_DOUBLE, prev_rank, 0, comm, &request_recv2);


      MPI_Wait(&request_recv2, MPI_STATUS_IGNORE);
      MPI_Wait(&request_recv, MPI_STATUS_IGNORE);

      //update on the first row, i = 1
      for (j = 1; j < m - 1; j++) {
        M.new_field[1 * m + j] =
            0.25*(M.field[(1 + 1) * m + j] + M.field[(1 - 1) * m + j] +
                      M.field[1 * m + j + 1] + M.field[1 * m + j - 1]);
      }

      
      //update on the first row, i = n-2
      for (j = 1; j < m - 1; j++) {
        M.new_field[(n-2) * m + j] =
            0.25*(M.field[((n-2) + 1) * m + j] + M.field[((n-2) - 1) * m + j] +
                      M.field[(n-2) * m + j + 1] + M.field[(n-2) * m + j - 1]);
      }


      MPI_Wait(&request_send, MPI_STATUS_IGNORE);
      MPI_Wait(&request_send2, MPI_STATUS_IGNORE);
      
      //print to file at multiples of PrintInterval
      if(step%PrintInterval == 0){
        std::ostringstream temp;
        temp << "./data/" << std::setw(5) << std::setfill('0') << step <<".dat";
        std::ofstream filevar; 
 
        filevar.open(temp.str(), std::ios::app);

        if (!my_rank){          
          //first print p0 block
          for(size_t i = 0; i < n-1; i++){
            for(size_t j = 0; j < m; j++){
              filevar << M.new_field[i * m + j] << " "; 
            }
            filevar << std::endl;
          }
          
          //receive blocks from other processes
          for (int rank = 1; rank < comm_sz; rank++){
            //compute how many rows each rank will send. We are not receiving ghost cells.
            size_t rows_from_rank = (rank < rem) ? local_rows + 1: local_rows;
            M.print_field.resize(rows_from_rank*m);
            MPI_Recv(M.print_field.data(), rows_from_rank*m, MPI_DOUBLE, rank, 2, comm, MPI_STATUS_IGNORE);

            //append to the .txt file right after receiving
            for(size_t i = 0; i < rows_from_rank; i++){
              for(size_t j = 0; j < m; j++){
                filevar << M.print_field[i * m + j] << " "; 
              }
              filevar << std::endl;
            }
        }

        filevar.close();

        } else {
          //just compute the local rows here again instead of passing from main. We are not sending ghost cells.
          size_t rows_to_handle = (my_rank < rem) ? local_rows + 1: local_rows;
          MPI_Send(M.new_field.data() + m, rows_to_handle*m, MPI_DOUBLE, 0, 2, comm);
        }

      }

      M.new_field.swap(M.field);

      step++;
    }//while
  }//jacobi
};//CSolver

#endif
