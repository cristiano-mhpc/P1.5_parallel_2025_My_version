#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int sendbuf[4], recvbuf[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        printf("This example requires exactly 4 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize send buffer with unique values
    for (int i = 0; i < size; i++) {
        sendbuf[i] = rank * 10 + i;
    }
    
    //print my data
    printf("From rank %d: \n", rank);
    for (int i = 0; i < size; i++){
	printf("%d  ", sendbuf[i]);
    }
   
    printf("\n");
   
    // Perform all-to-all communication
    MPI_Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD);

    // Print out received values for each process
    printf("Process %d received values:", rank);
    for (int i = 0; i < size; i++) {
        printf(" %d", recvbuf[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}

