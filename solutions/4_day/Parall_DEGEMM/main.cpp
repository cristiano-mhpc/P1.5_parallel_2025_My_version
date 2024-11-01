#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <cblas.h>

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
		std::cout << "Usage: Enter the size of the square matrix from the command line. " << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	/**
	 * Get the number number of rows from the user.
	*/
	size_t N = std::stoi(argv[1]);

	if (N%comm_sz != 0){
		std::cout << "Usage: the dimension of the square matrix must be a multiple of number of communicator."
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

	//a temporary vector for holding partial matmul results in the blocks. relevant in degemm.
	std::vector<double> temp_C_loc(N_loc*N_loc);
	
	//the main loop 
	for(size_t i = 0; i < comm_sz; i++){
		/**
		 * Gather the ith block of B. Since the block each process will contribute
		 * is not contiguous member of their B_loc, we need to pack it first.
		*/
	
		for(size_t k = 0; k < N_loc; k++){
			for (size_t j = 0; j < N_loc; j++){
				//THINK IF WE REALLY NEED start_row here COZ WE'RE JUST WORKING ON B_loc
				//B_pack(k * N_loc + j) = B_loc( ( (k + start_row) * N_loc) + (i * N_loc) + j );
				B_pack[k * N_loc + j] = B_loc[ ( (k) * N_loc) + (i * N_loc) + j ];
			}
		}
	
		//now get my B_block
		MPI_Allgather(B_pack.data(), N_loc*N_loc, MPI_DOUBLE, B_block.data(), N_loc*N_loc, MPI_DOUBLE, MPI_COMM_WORLD);
		//after getting my B_block, compute the corresponding block of my C_local
		//perform C_local_block = Mult(A_loc, B_block);

		#ifdef NAIVE
		for (size_t k = 0; k < N_loc ; k++){
			for (size_t j = 0; j < N_loc; j++){
				double sum = 0.0;
				for (size_t l = 0; l < N; l++){
					sum += A_loc[ k*N + l] * B_block[ l * N_loc + j ];
				}
				C_loc[ (k)*(N) +  i*N_loc + j] = sum;
			}
		}
		#endif

		#ifdef DGEMM		
		// Call dgemm
   		// C = alpha * A * B + beta * C
    	// A: MxK matrix
    	// B: KxN matrix
    	// C: MxN matrix

		/**********************************
		 * cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                M, N, K, 
                alpha, 
                A.data(), K, 
                B.data(), N, 
                beta, 
                C.data(), N);
		***********************************/
		
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N_loc, N_loc, N, 1, A_loc.data(), N, B_block.data(), N_loc, 0, temp_C_loc.data(), N_loc);

		for (size_t k = 0; k < N_loc ; k++){
			for (size_t j = 0; j < N_loc; j++){
				C_loc[ (k)*(N) +  i*N_loc + j] = temp_C_loc[k*N_loc + j];
			}
		}

		#endif
	}//for
	
	
	//have process 0 print the result 
	if (!my_rank){
		//print p0 C_local
		for (size_t i = 0; i < N_loc; i++){
			for (size_t j=0; j < N; j++){
				std::cout << C_loc[i*N + j] << " ";
			}
			std::cout << std::endl;
		}
		
		if(comm_sz > 1){
			C_print.resize(N_loc*N);

			for (size_t rank = 1; rank < comm_sz; rank++){
				MPI_Recv(C_print.data(), N_loc*N, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
				for (size_t i = 0; i < N_loc; i++){
					for (size_t j=0; j < N; j++){
						std::cout << C_print[i*N + j] << " ";
					}
					std::cout << std::endl;
				}
			}
		}//if
		
	} else {
		MPI_Send(C_loc.data(), N_loc*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	
	/*
	std::cout << "From rank " << my_rank << std::endl;
	for (size_t i = 0; i < N_loc; i++){
		for (size_t j=0; j < N; j++){
			std::cout << C_loc[i*N + j] << " ";
		}
		std::cout << std::endl;
	}
	*/

	MPI_Finalize();
	return 0;

}
