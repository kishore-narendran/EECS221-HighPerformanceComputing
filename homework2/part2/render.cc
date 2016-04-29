/**
 *  \file render.cc
 *
 *  \brief Implements a routine for rendering images. See 'render.hh'.
 */

#include <cassert>
#include <iostream>
#include <cstdlib>

#include "render.hh"

/** Construct a color suitable for display. */
gil::rgb8_pixel_t render(float v) {
  // Use smooth polynomials for r, g, b
  int r = (int)(9*(1-v)*v*v*v*255);
  int g = (int)(15*(1-v)*(1-v)*v*v*255);
  int b =  (int)(8.5*(1-v)*(1-v)*(1-v)*v*255);
  return gil::rgb8_pixel_t(r, g, b);
}

/* eof */
