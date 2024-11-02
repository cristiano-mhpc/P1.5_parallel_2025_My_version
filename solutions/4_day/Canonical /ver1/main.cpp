#include <mpi.h>
#include <iostream>

int main(int argc, char** argv){

	int comm_sz, my_rank;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	/**
	 * The dimensions of the Cartesian grid
	 */
	//we have a 2d plane
	int ndims = 2;
	//MPI will decide the dimensions 
	int dimensions[2] = {0, 0};

	//create the dimensions
	MPI_Dims_create(comm_sz, ndims, dimensions);

	//specify periodic along both direction
	int periodicity[2] = {1,1};

	//handle for the cartesian communicator
	MPI_Comm cart_comm;

	//create the cartesian grid of process
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dimensions, periodicity, 1, &cart_comm);

	//get my coordinate in the cartesian grid
	int coords[2];
	MPI_Cart_coords(cart_comm, my_rank, ndims, coords);

	//query the rank of my neighbors. Very handy when exchanging
	int up, down, left, right;
	MPI_Cart_shift(cart_comm, 0, 2, &up, &down); //shift 2 along the row index(vertical axis)
	MPI_Cart_shift(cart_comm, 1, 2, &left, &right); //shift 2 along the column index(horizontal axis)

	//print the info about the process 
	    std::cout << "Rank: " << my_rank 
              << " Coordinates: (" << coords[0] << ", " << coords[1] << ") "
              << "2-shift Neighbors - Left: " << left 
              << ", Right: " << right 
              << ", Up: " << up 
              << ", Down: " << down 
              << std::endl;
	
	MPI_Comm_free(&cart_comm);
	
	MPI_Finalize();
	return 0;

}
