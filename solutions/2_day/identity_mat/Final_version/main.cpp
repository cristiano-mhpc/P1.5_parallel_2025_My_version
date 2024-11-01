#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char** argv) {
    int my_rank, comm_sz;
    int N = 19;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Calculate the local number of rows each process will handle
    int local_N = N / comm_sz;
    int rem = N % comm_sz;  // Number of remaining rows

    /**
     * Determine the number of rows this process will handle. Remaining 
     */
    int rows_to_handle = (my_rank < rem) ? local_N + 1 : local_N;
    std::vector<int> A(rows_to_handle * N, 0);

    // Determine the global start row index for this process
    int start_row = my_rank * local_N + std::min(my_rank, rem);

    // Populate the local identity matrix rows
    for (int i = 0; i < rows_to_handle; i++) {
        int global_i = start_row + i;
        A[i * N + global_i] = 1;
    }
    /*
    std::cout << "From rank " << my_rank << " my matrix is: " << std::endl;
    for (int i = 0; i < rows_to_handle; i++){
		for (int j = 0; j < N; j++){
			std::cout << A[i*(N) + j] <<  " ";
		}
		std::cout << std::endl;
	}

    */

    //send each block to process 0
    if (my_rank == 0){

        std::ofstream outFile("mat.bin", std::ios::binary | std::ios::app);
        /*
        for (int i = 0; i < rows_to_handle; i++){
		    for (int j = 0; j < N; j++){
			    std::cout << A[i*(N) + j] <<  " ";
		    }
		    std::cout << std::endl;
	    }
        */
        outFile.write(reinterpret_cast<const char*>(A.data()), A.size() * sizeof(int));

        std::vector<int> B(N*rows_to_handle);
        //recieve the rows from the other processese
        for (int rank = 1; rank < comm_sz; rank++){

            int rows_from_rank = (rank < rem) ? local_N + 1: local_N;
            MPI_Recv(B.data(), rows_from_rank * N, MPI_INT, rank, 0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //print what I receive

            outFile.write(reinterpret_cast<const char*>(B.data()), rows_from_rank * N * sizeof(int));
            /*
            //std::cout << "From rank " << rank << " received the matrix: " << std::endl;
            for (int i = 0; i < rows_from_rank; i++){
		        for (int j = 0; j < N; j++){
			        std::cout << B[i*(N) + j] <<  " ";
		        }
		        std::cout << std::endl;
	        }
            */

        }
        outFile.close();

    } else {
        MPI_Send(A.data(), rows_to_handle * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}

