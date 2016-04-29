#if !defined (INC_RENDER_HH)
#define INC_RENDER_HH

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>

namespace gil = boost::gil;

/** Construct a color suitable for display. */
gil::rgb8_pixel_t render(float v);

#endif
