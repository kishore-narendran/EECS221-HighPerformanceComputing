/**
 *  \file mandelbrot_serial.cc
 *  \brief Lab 2: Mandelbrot set serial code
 */


#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <math.h>

#include "timer.c"
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

  struct stopwatch_t* timer;
  timer = stopwatch_create ();
  stopwatch_init ();
  stopwatch_start (timer);

  //MPI Initialization
  int rank=0, np=0, namelen=0;
  char hostname[MPI_MAX_PROCESSOR_NAME+1];

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* Get process id */
  MPI_Comm_size (MPI_COMM_WORLD, &np);	/* Get number of processes */
  MPI_Get_processor_name (hostname, &namelen); /* Get hostname of node */
  //printf ("Hello, world! [Host:%s -- Rank %d out of %d]\n", hostname, rank, np);

  if(rank == 0)
  {
    printf("Mandelbrot Image Generation using Susie Cyclic's Logic started!\n");
  }
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

  //Rounding up value of height and width
  height = round((float)height/(float)np) * np;
  width = round((float)width/(float)np) * np;

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
  float *recv_buffer = new float [width*height];
  for(int i = 0; i < height; i++)
  {
    final_image[i] = new float[width];
  }

  //Mandelbrot parallel code here
  float *local_mandelbrot_values = new float[(height*width)/np];
  y = minY + rank*it;
  for(int i = 0; i < height/np; ++i)
  {
    x = minX;
    for(int j = 0; j < width; ++j)
    {
      local_mandelbrot_values[i*width+j] = (mandelbrot(x,y)/512.0);
      x += jt;
    }
    y += it*np;
  }

  //Gathering
  MPI_Gather(local_mandelbrot_values, (height/np)*width, MPI_FLOAT, recv_buffer, (height/np)*width, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if(rank == 0)
  {
    int process_block = 0;
    for (int i = 0; i < height; i++)
    {
      if(i%np == 0)
      {
        process_block = (i/np);
      }
      for(int j=0; j < width; j++)
      {
        final_image[i][j] = recv_buffer[(process_block*width)+j];
        img_view(j,i) = render(final_image[i][j]);
      }
      process_block = process_block + height/np;
    }
    char *filename = new char[50];
    sprintf(filename, "mandelbrot_susie_%d_%dx%d.png", np, height, width);
    gil::png_write_view(filename, const_view(img));
  }
  MPI_Finalize();
  long double elap_time = stopwatch_stop (timer);
  stopwatch_destroy (timer);
  if(rank == 0)
  {
    printf ("Time: %Lg seconds",elap_time);
    printf("Generating image of size %dx%d using %d processes\n", height, width, np);
    printf("Mandelbrot Image Generation using Joe Block's Logic finished!\n\n");
  }
  return 0;
}

/* eof */
