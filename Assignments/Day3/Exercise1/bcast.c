#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main( int argc, char * argv[] ){

//  int imesg = 0;
  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes
  size_t size = 1024 * 1024 * 1024;
  int n_elem = size / sizeof(int);
  int* buffer = (int*)malloc(size);

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  for (int i=0; i < n_elem; ++i) {
      buffer[i] = -npes;
  }

//  imesg = rank;
  fprintf( stderr, "\nBefore Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, buffer[npes - 1] );

  MPI_Bcast( buffer, n_elem, MPI_INT, 0, MPI_COMM_WORLD );

  fprintf( stderr, "\nAfter Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, buffer[npes - 1] );

  MPI_Finalize();
  
  return 0;

}
