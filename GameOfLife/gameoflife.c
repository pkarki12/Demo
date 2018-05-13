// gameoflife.c
// Name: Prasiddha Karki
// JHED: pkarki1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mpi.h"

#define DEFAULT_ITERATIONS 64
#define GRID_WIDTH  256
#define DIM  16     // assume a square grid

// Returns the value of the cell in the next iteration
int cellVal(int temp_grid[], int index) {

  int a, b; // Offsets for boundary conditions
  if (index % 16 == 0) { // If on leftmost column
    a = 16;
  } else {
    a = 0;
  }
  if (index % 16 == 15) { // If on rightmost column
    b = 16;
  } else {
    b = 0;
  }

  // Sums number of live neighbors
  int neighbors = temp_grid[index - 17 + a] + temp_grid[index - 16] +
              temp_grid[index - 15 - b] + temp_grid[index - 1 + a] +
              temp_grid[index + 1 - b] + temp_grid[index + 15 + a] +
              temp_grid[index + 16] + temp_grid[index + 17 - b];

  // Determines new value for cell based on number of live neighbors
  if (temp_grid[index] == 0 && neighbors == 3) {
    return 1;
  } else if (temp_grid[index] == 1 && !(neighbors == 2 || neighbors == 3)) {
    return 0;
  } else {
    return temp_grid[index];
  }
}

int main ( int argc, char** argv ) {

  int global_grid[GRID_WIDTH] =
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  // MPI Standard variables
  int num_procs;
  int ID, j;
  int iters = 0;
  int num_iterations;

  // Setup number of iterations
  if (argc == 1) {
    num_iterations = DEFAULT_ITERATIONS;
  }
  else if (argc == 2) {
    num_iterations = atoi(argv[1]);
  }
  else {
    printf("Usage: ./gameoflife <num_iterations>\n");
    exit(1);
  }

  // Messaging variable
  MPI_Status stat;

  // MPI Setup
  if ( MPI_Init( &argc, &argv ) != MPI_SUCCESS )
  {
    printf ( "MPI_Init error\n" );
  }

  MPI_Comm_size ( MPI_COMM_WORLD, &num_procs ); // Set the num_procs
  MPI_Comm_rank ( MPI_COMM_WORLD, &ID );

  assert ( DIM % num_procs == 0 );

  int temp_grid[GRID_WIDTH + 32]; // Temporary grid to work with
  int chunk = GRID_WIDTH / num_procs; // Size of grid for each task to work on
  int offset; // First index position of grid for each task to work on

  for ( iters = 0; iters < num_iterations; iters++ ) {

    // Master tasks
    if (ID == 0) {
      // Initialize the temporary grid
      // First row is last row of global_grid
      // Last row is first row of global_grid
      for (int i = 0; i < 16; i++) {
        temp_grid[i] = global_grid[GRID_WIDTH - 16 + i];
      }
      for (int i = 16; i < GRID_WIDTH + 16; i++) {
        temp_grid[i] = global_grid[i - 16];
      }
      for (int i = GRID_WIDTH + 16; i < GRID_WIDTH + 32; i++) {
        temp_grid[i] = global_grid[i - GRID_WIDTH - 16];
      }

      // Send each task its chunk of the temporary grid to work with
      offset = chunk;
      for (int i = 1; i < num_procs; i++) {
        MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        MPI_Send(&temp_grid[offset], chunk + 32, MPI_INT, i, 2, MPI_COMM_WORLD);
        offset = offset + chunk;
      }

      // Master works on the first chunk
      offset = 0;
      for (int i = offset; i < offset + chunk; i++) {
        global_grid[i] = cellVal(temp_grid, i + 16);
      }

      // Recieve the chunks of the global_grid from the workers after they are done
      for (int i = 1; i < num_procs; i++) {
        MPI_Recv(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
        MPI_Recv(&global_grid[offset], chunk, MPI_INT, i, 2, MPI_COMM_WORLD, &stat);
      }

      // Print each iteration of global_grid
      printf ( "\nIteration %d: final grid:\n", iters );
      for ( j = 0; j < GRID_WIDTH; j++ ) {
        if ( j % DIM == 0 ) {
          printf( "\n" );
        }
        printf ( "%d  ", global_grid[j] );
      }
      printf( "\n" );
    }

    // Workers' tasks
    if (ID > 0) {
      // Receive chunk of the temporary grid to work with
      MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat);
      MPI_Recv(&temp_grid[offset], chunk + 32, MPI_INT, 0, 2, MPI_COMM_WORLD, &stat);

      // Do work for given chunk
      for (int i = offset; i < offset + chunk; i++) {
        global_grid[i] = cellVal(temp_grid, i + 16);
      }

      // Send the results back to the master
      MPI_Send(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
      MPI_Send(&global_grid[offset], chunk, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize(); // finalize so I can exit
}