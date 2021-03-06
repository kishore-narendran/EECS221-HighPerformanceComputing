/**
 *  \file mandelbrot_serial.cc
 *  \brief Lab 2: Mandelbrot set serial code
 */


#include <iostream>
#include <cstdlib>

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

  printf("Mandelbrot Image Generation Serially started!\n");
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

  y = minY;
  for (int i = 0; i < height; ++i) {
    x = minX;
    for (int j = 0; j < width; ++j) {
      img_view(j, i) = render(mandelbrot(x, y)/512.0);
      x += jt;
    }
    y += it;
  }
  char *filename = new char[50];
  sprintf(filename, "mandelbrot_serial_%dx%d.png", height, width);
  gil::png_write_view(filename, const_view(img));

  long double elap_time = stopwatch_stop (timer);
  stopwatch_destroy (timer);
  printf ("Time: %Lg seconds",elap_time);
  printf("Generating image of size %dx%d using one process\n", height, width);
  printf("Mandelbrot Image Generation Serially finished!\n\n");
  return 0;
}

/* eof */
