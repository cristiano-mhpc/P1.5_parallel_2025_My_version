#ifndef BOUNDCOND
#define BOUNDCOND

#include <vector>


template <typename T>
void boundary_cond(std::vector<T>& V, size_t N){
   /**
   *A templated function that is used to set the 
   *initial condition for the field and the new_field
   * attributes of the CMesh class. 
   */
   size_t i,j;
  
   //initialize the bottom row
   for (j = 0; j < N - 1; j++){
     V[(N-1)*(N) + j] = 100 - (100/(N))*j;
   }
 
  
   //initialize the first column
   for(i = 1; i < N - 1; i++){
     V[i*N] = V[N*(N-1) + ((N - 1) - i)];
   }
   
   //populate everywhere else with 0.5 
  for(i = 1; i < N-1 ; i++){
    for(j = 1 ; j < N-1 ;j++){
      V[i*N + j] = 0.5;
    }
  }

}

#endif

