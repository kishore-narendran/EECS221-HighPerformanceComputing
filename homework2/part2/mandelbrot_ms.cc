/**
 *  \file mandelbrot_ms.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

 #include <iostream>
 #include <cstdlib>
 #include <mpi.h>

 #include "render.hh"

 using namespace std;

 #define WIDTH 1000
 #define HEIGHT 1000

 int mandelbrot(double x, double y)
 {
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


int main (int argc, char* argv[])
{
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
  if (argc == 3)
  {
    height = atoi (argv[1]);
    width = atoi (argv[2]);
    assert (height > 0 && width > 0);
  }
  else
  {
    fprintf (stderr, "usage: %s <height> <width>\n", argv[0]);
    fprintf (stderr, "where <height> and <width> are the dimensions of the image.\n");
    return -1;
  }

  double it = (maxY - minY)/height;
  double jt = (maxX - minX)/width;
  double x = minX, y = minY;
  gil::rgb8_image_t img(height, width);
  auto img_view = gil::view(img);

  /* Lucky you, you get to write MPI code */

  int rows_received = 0, rows_sent = 0;
  float **final_image = new float *[height];
  if(rank == 0)
  {
    for(int i = 1; i < np; i++)
    {
      MPI_Send(&rows_sent, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      y += it;
      rows_sent += 1;
    }

    while(rows_received < height)
    {
      float *recv_buffer = new float[width];
      MPI_Status status;
      MPI_Recv(recv_buffer, width+1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      rows_received++;
      int received_from = status.MPI_SOURCE;
      int current_row = (int) recv_buffer[0];
      final_image[current_row] = new float[width];
      for(int j = 0; j < width; j++)
      {
        final_image[current_row][j] = recv_buffer[j+1];
      }
      if(rows_sent < height)
      {
        MPI_Send(&rows_sent, 1, MPI_INT, received_from, 0, MPI_COMM_WORLD);
        rows_sent++;
      }
      else
      {
        rows_sent = height;
        MPI_Send(&rows_sent, 1, MPI_INT, received_from, 0, MPI_COMM_WORLD);
      }
    }
    //Rendering the image
    for (int i = 0; i < height; ++i)
    {
      for (int j = 0; j < width; ++j)
      {
        img_view(j, i) = render(final_image[i][j]);
      }
    }
    gil::png_write_view("mandelbrot_ms.png", const_view(img));
    MPI_Finalize();
    return 0;
  }
  else
  {
    while(true)
    {
      //Slave receives the row value to work on
      int slave_row = 0;
      double slave_y = 0;
      double slave_x = minX;
      MPI_Recv(&slave_row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if(slave_row == height)
      {
        break;
      }
      slave_y = minY + it * slave_row;
      float *slave_mandelbrot_values = new float[width + 1];
      slave_mandelbrot_values[0] = slave_row;
      //Slave computer values for yth row
      for(int j = 0; j < width; j++)
      {
        slave_mandelbrot_values[j + 1] = (mandelbrot(slave_x,slave_y)/512.0);
        slave_x += it;
      }
      MPI_Send(slave_mandelbrot_values, width, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
    //Slave code goes here
  }
}

/* eof */
