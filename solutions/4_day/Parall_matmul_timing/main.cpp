#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include "Parallel_CSimple_timer.hpp"

int main(int argc, char** argv){

	int my_rank, comm_sz;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/**
	 * Here we assume that the matrices A, B, and C are
	 * square of size N. N is a multiple of comm_sz;
	*/
	
	if (argc != 2){
		std::cout << "USAGE: Enter the size of the square matrix from the command line. " << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	/**
	 * Get the number number of rows from the user.
	*/
	size_t N = std::stoi(argv[1]);

	if (N%comm_sz != 0){
		if (!my_rank){
			std::cout << "USAGE: size of the square matrix must be a multiple of number of process." << std::endl;
		}
		
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	//local number of rows
	size_t N_loc = N/comm_sz;

	//initialize the vector containers for the matrices relevant to this process
	std::vector<double> A_loc(N_loc*N, 1.0);
	std::vector<double> B_loc(N_loc*N, 1.0);
	std::vector<double> C_loc(N_loc*N);
	std::vector<double> B_block(N*N_loc);
	//The vector for packing my non-contiguous contribution to B_block.  
	std::vector<double> B_pack(N_loc*N_loc);

	//the vector the contain the results of others and print them
	std::vector<double> C_print(N_loc*N);
	
	//the main loop
	{ 
		CSimple_timer t1("Total", my_rank, comm_sz, MPI_COMM_WORLD);
		for(size_t i = 0; i < comm_sz; i++){
			/**
		 	* Gather the ith block of B. Since the block each process will contribute
		 	* is not contiguous member of their B_loc, we need to pack it first.
			*/
		
			for(size_t k = 0; k < N_loc; k++){
				for (size_t j = 0; j < N_loc; j++){
					B_pack[k * N_loc + j] = B_loc[ ( (k) * N_loc) + (i * N_loc) + j ];
				}
			}
			
			{
				CSimple_timer t2("comm", my_rank, comm_sz, MPI_COMM_WORLD);
				//now get my B_block
				MPI_Allgather(B_pack.data(), N_loc*N_loc, MPI_DOUBLE, B_block.data(), N_loc*N_loc, MPI_DOUBLE, MPI_COMM_WORLD);
			}

			for (size_t k = 0; k < N_loc ; k++){
				for (size_t j = 0; j < N_loc; j++){
					double sum = 0.0;
					for (size_t l = 0; l < N; l++){
						sum += A_loc[ k*N + l] * B_block[ l * N_loc + j ];
					}
					C_loc[ (k)*(N) +  i*N_loc + j] = sum;
				}
			}
		}//for
	}
	
	CSimple_timer::print_timing_results(my_rank, comm_sz, MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;

}
