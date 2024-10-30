#ifndef CMESH
#define CMESH

#include <vector>
#include <iostream>
#include <fstream>
#include <mpi.h>

extern void some_bound_cond();

template <typename T>
class CMesh{

  public:
  size_t rows;
  size_t colm;
  std::vector<T> field;
  std::vector<T> new_field;

  //declare a printing method
  void print(std::ostream& os, std::vector<T> V, size_t M);

  void test_print(){
      for(int i = 0; i < rows; i++){
        for (int j = 0; j < colm; j++){
          std::cout << field[i*colm + j] << " ";
        }
        std::cout << std::endl;
      }
  }

  //constructor
  //template <typename F>
  CMesh(size_t rows1, size_t colm1, size_t total_rows, int rank, int comm_sz, size_t start_row, MPI_Comm comm):rows(rows1), colm(colm1){
    field.resize(rows*colm);
    new_field.resize(rows*colm);
    double delta = 100.0/(total_rows-1); 

    if (rank == 0){
      //initialize the body of the head
      for (int i=1; i < rows-1; i++){
        for (int j=1; j < colm-1; j++){
          new_field[i*colm + j] = 0.5;
          field[i*colm + j] = 0.5;
        }
      }

      //initialize the first column of the head
      for(int i = 1; i < rows-1; i++){
        new_field[i*colm] = 0 + i*delta;
        field[i*colm] = 0 + i*delta;
      }

    } else if (rank == comm_sz - 1){
      //initialize the body of the tail. 
      for (int i = 1; i < rows-1; i++){
        for (int j = 1; j < colm-1; j++){
          field[i*colm + j] = 0.5;
          new_field[i*colm + j] = 0.5;
        }
      }

      //initialize the first column of the tail
      for(int i = 1; i < rows; i++){
        new_field[i*colm] = start_row*delta + (i-1)*delta;
        field[i*colm] = start_row*delta + (i-1)*delta;
      }

      //initialize the last row of the tail
      for(int j = 1; j < colm; j ++){
        new_field[(rows-1)*colm + j] = 100.0 - j*delta;
        field[(rows-1)*colm + j] = 100.0 - j*delta;
      }


    } else {
      //initialize the center part of the body
      for (int i = 1; i < rows-1; i++){
        for (int j = 1; j < colm-1; j++){
          new_field[i*colm + j] = 0.5;
          field[i*colm + j] = 0.5;
        }
      }

      //initialize the first column of the body 
      for (int i = 1; i < rows-1; i++){
        new_field[i*colm] = start_row*delta + (i-1)*delta;
        field[i*colm] = start_row*delta + (i-1)*delta;
      }

    }


   }//constructor
 
};

template <typename L>
void CMesh<L>::print(std::ostream& os, std::vector<L> V, size_t M){
  size_t i, j;
  for(i = 0; i < M; i++){
   for(j = 0; j < M; j++){
     os << V[i * M + j] << " "; 
   }
   os << std::endl;
  }
}

#endif
