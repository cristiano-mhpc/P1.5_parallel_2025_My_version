#ifndef CMESH
#define CMESH

#include <vector>
#include <iostream>
#include <fstream>
#include "BoundCond.hpp"

extern void some_bound_cond();

template <typename T>
class CMesh{
  public:
  size_t N;
  std::vector<T> field;
  std::vector<T> new_field;
  //decalre a printing method
  void print(std::ostream& os, std::vector<T> V, size_t M);

  //constructor
  template <typename F>
  CMesh(size_t N1, F some_bound_cond):N(N1){
    field.resize(N*N);
    new_field.resize(N*N);

    //SETUP THE BOUNDARY CONDITION HERE
    some_bound_cond(field, N);
    some_bound_cond(new_field, N);
   }
 
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
