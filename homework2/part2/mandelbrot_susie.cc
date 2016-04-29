/**
 *  \file mandelbrot_susie.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>
using namespace std;
int main (int argc, char* argv[])
{
  int rank=0;
  int np=0;
  int namelen=0;
  char hostname[MPI_MAX_PROCESSOR_NAME+1];
  /* Lucky you, you get to write MPI code */
  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* Get process id */
  MPI_Comm_size (MPI_COMM_WORLD, &np);	/* Get number of processes */
  MPI_Get_processor_name (hostname, &namelen); /* Get hostname of node */
  printf ("Hello, world! [Host:%s -- Rank %d out of %d]\n", hostname, rank, np);
  int *rbuf = new int[np*10];
  int *data = new int[10];
  for(int j = 0; j < 10; j++)
  {
    data[j] = rank;
  }
  MPI_Gather(data,10,MPI_INT,rbuf,10,MPI_INT,0,MPI_COMM_WORLD);
  if(rank==0)
  {
     cout<<"This is the root"<<endl;
     for(int j = 0; j < np*10; j++)
     {
	cout<<rbuf[i]<<" ";
     }
     cout<<endl;
  }
  delete data;
  delete rbuf;
  MPI_Finalize();
  return 0;
}

/* eof */
