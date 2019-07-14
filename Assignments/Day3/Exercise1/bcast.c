#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

/* Main function */

int main( int argc, char * argv[] ){

  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes
  size_t size = 1024 * 1024 * 1024;  // just more than 1 GB of size for the buffer
  int n_elem = size / sizeof(int);  // number of elements in the buffer
  int* buffer = (int*)malloc(size); // allocate the buffer

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  // initialize the buffer to be -NUM_PROCS_IN_MPI_COMM_WORLD
  for (int i=0; i < n_elem; ++i) {
      buffer[i] = -npes;
  }

  fprintf( stderr, "\nBefore Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, buffer[npes - 1] );
  
  // perform 1:n communication from process 0
  MPI_Bcast( buffer, n_elem, MPI_INT, 0, MPI_COMM_WORLD );

  fprintf( stderr, "\nAfter Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, buffer[npes - 1] );

  // finalize
  MPI_Finalize();
  
  return 0;

}
