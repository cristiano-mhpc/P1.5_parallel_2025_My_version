#include <mpi.h>
#include <iostream>
#include <vector>

//void print(int rank, int comm_size, MPI_comm){



//}


int main(int argc, char** argv){

	int my_rank;
	int comm_sz;
	int N = 11;	
	
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	//local number of rows 
	int local_N = (my_rank < rem ) ? N/comm_sz+1:N/comm_sz;
	std::vector<int> A(N* local_N, 0);
	//the remainder rows
	int rem = N%comm_sz;  
	

	if (my_rank < rem){
		A.resize(N * local_N); 
		//populate the matrix with offset = 0 
		//int global_i = 0;
		for (int i = 0; i < local_N; i++){
			int global_i = i + (local_N * my_rank);
			A[i * N + global_i] = 1;
		}

	}else{
		A.resize(N*local_N);
		int offset = rem;
		int global_i = 0;
		//popuate the matrix with offset = rem;
		for (int i = 0; i < local_N; i++){
			global_i = i + (local_N * my_rank) + offset;
			for (int j = 0; j < N; j++){
				if (global_i == j){
					A[i*(N) + j] = 1;
				} else {
					A[i*(N) + j] = 0;
				}
			}
		}
	} 

	for (int i = 0; i < local_N + 1; i++){
		for (int j = 0; j < N; j++){
			std::cout << A[i*(N) + j] <<  " ";
		}
		std::cout << std::endl;
	}

	MPI_Finalize();

	return 0;



}
