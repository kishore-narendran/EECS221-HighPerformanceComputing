/**
 *  \file mandelbrot_serial.cc
 *  \brief Lab 2: Mandelbrot set serial code
 */


#include <iostream>
#include <cstdlib>
#include <mpi.h>

#include "render.hh"

using namespace std;

#define WIDTH 1000
#define HEIGHT 1000

int
mandelbrot(double x, double y) {
  int maxit = 511;
  double cx = x;
  double cy = y;
  double newx, newy;

  int it = 0;
  for (it = 0; it < maxit && (x*x + y*y) < 4; ++it) {
    newx = x*x - y*y + cx;
    newy = 2*x*y + cy;
    x = newx;
    y = newy;
  }
  return it;
}

int
main(int argc, char* argv[]) {
  //MPI Initialization
  int rank=0, np=0, namelen=0;
  char hostname[MPI_MAX_PROCESSOR_NAME+1];

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* Get process id */
  MPI_Comm_size (MPI_COMM_WORLD, &np);	/* Get number of processes */
  MPI_Get_processor_name (hostname, &namelen); /* Get hostname of node */
  printf ("Hello, world! [Host:%s -- Rank %d out of %d]\n", hostname, rank, np);

  //Mandelbrot Code
  double minX = -2.1;
  double maxX = 0.7;
  double minY = -1.25;
  double maxY = 1.25;

  int height, width;
  if (argc == 3) {
    height = atoi (argv[1]);
    width = atoi (argv[2]);
    assert (height > 0 && width > 0);
  } else {
    fprintf (stderr, "usage: %s <height> <width>\n", argv[0]);
    fprintf (stderr, "where <height> and <width> are the dimensions of the image.\n");
    return -1;
  }

  double it = (maxY - minY)/height;
  double jt = (maxX - minX)/width;
  double x, y;

gil::rgb8_image_t img(height, width);
auto img_view = gil::view(img);

  /*
  //Mandelbrot serial code is here
  y = minY;
  for (int i = 0; i < height; ++i) {
    x = minX;
    for (int j = 0; j < width; ++j) {
      img_view(j, i) = render(mandelbrot(x, y)/512.0);
      x += jt;
    }
    y += it;
  }
  */

  //Creating a receiver buffer
  float **final_image = new float *[height];
  float **recv_buffer = new float *[height];
  for(int i = 0; i < height; i++)
  {
    recv_buffer[i] = new float[width];
    final_image[i] = new float[width];
  }

  //Mandelbrot parallel code here
  float **local_mandelbrot_values = new float*[height/np];
  y = minY + rank*(height/np)*it;
  for(int i = 0; i < height/np; ++i)
  {
    x = minX;
    local_mandelbrot_values[i] = new float[width];
    for(int j = 0; j < width; ++j)
    {
      local_mandelbrot_values[i][j] = (mandelbrot(x,y)/512.0);
      x += jt;
    }
    y += it;
  }

  //Gathering
  MPI_Gather(local_mandelbrot_values, (height/np)*width, MPI_FLOAT, recv_buffer, (height/np)*width, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if(rank == 0)
  {
    for (int i = 0; i < height; ++i)
    {
      for (int j = 0; j < width; ++j)
      {
        img_view(j, i) = render(recv_buffer[i][j]);
      }
    }
    gil::png_write_view("mandelbrot.png", const_view(img));
  }
  MPI_Finalize();
  return 0;
}

/* eof */
