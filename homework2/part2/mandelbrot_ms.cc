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

  int current_row = 0, rows_sent = 0;
  float **final_image = new float *[height];
  if(rank == 0)
  {
    //Master code goes here
    while(true) //While there are rows of image work to be done
    {
      for(int i = 1; i < np; i++)
      {
        if(rows_sent == height)
        {
          //Marking i value for which the processing was finished
          break;
        }
        //Send slaves units of work - by designating the y
        MPI_Send(&y, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        y += it;
        rows_sent++;
      }
      for(int i = 1; i < np; i++)
      {
        if(current_row == height)
        {
          break;
        }
        float *recv_buffer = new float[width];
        MPI_Recv(recv_buffer, width, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        final_image[current_row] = new float[width];
        for(int j = 0; j < width; j++)
        {
          final_image[current_row][j] = recv_buffer[j];
        }
        current_row++;
        //Receive from slaves the units of work completed
      }
      if(rows_sent == height || current_row == height)
      {
        //All rows of work have been sent and received
        break;
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
    gil::png_write_view("mandelbrot_joe.png", const_view(img));
    MPI_Finalize();
    return 0;
  }
  else
  {
    while(true)
    {
      //Slave receives the y value to work on
      double slave_y = 0;
      double slave_x = minX;
      MPI_Recv(&slave_y, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      float *slave_mandelbrot_values = new float[width];
      //Slave computer values for yth row
      for(int j = 0; j < width; j++)
      {
        local_mandelbrot_values[j] = (mandelbrot(slave_x,slave_y)/512.0);
        slave_x += it;
      }
      MPI_Send(slave_mandelbrot_values, width, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
    //Slave code goes here
  }
}

/* eof */
