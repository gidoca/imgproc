#include <complex>
#include <fstream>

#include "bmp.h"
#include "color_conversion.h"
#include "colors.h"
#include "image.h"

using image::Coordinate;
using image::Dimension;

using complex = std::complex<float>;

static constexpr auto max = 2.f;
static constexpr auto max_absolute_iter_value = 2.f;
static constexpr auto max_norm_iter_value =
    max_absolute_iter_value * max_absolute_iter_value;
static constexpr auto epsilon =
    std::numeric_limits<complex::value_type>::epsilon();

bmp::Image tonemap(image::ImageF32HSV const& im) {
  return im.map_pixels([](auto in_pixel) {
    using namespace color_conversion;
    return to_underlying<bmp::Image::PixelType>(to_srgb(from_hsv(in_pixel)));
  });
}

complex relative_pos(Coordinate c, Dimension dim) {
  return {(float)c.x / (float)(dim.width - 1),
          (float)c.y / (float)(dim.height - 1)};
}

complex coord_to_complex(Coordinate c, Dimension dim) {
  using namespace std::complex_literals;
  auto centered = relative_pos(c, dim) * 2.f - (1.f + 1.if);
  return max * centered;
}

complex calculate_iteration(complex x, complex c) { return x * x + c; }

bool has_escaped(complex val) {
  return norm(val) > max_norm_iter_value + epsilon;
}

pixel::PixelF32HSV color_lookup(size_t iteration, size_t num_iterations) {
  return {360.f * iteration / num_iterations, 1.f,
          iteration == num_iterations ? 0.f : 1.f};
}

image::ImageF32HSV calculate_mandelbrot(size_t num_iterations, Dimension dim) {
  image::ImageF32HSV out{dim};
  out.map_inplace(
      [num_iterations](auto& pixel, Coordinate coord, auto const& image) {
        const auto c = coord_to_complex(coord, image.dimension());
        complex val = 0;
        size_t iteration = 0;
        for (; iteration < num_iterations; ++iteration) {
          if (has_escaped(val)) break;
          val = calculate_iteration(val, c);
        }

        pixel = color_lookup(iteration, num_iterations);
      });
  return out;
}

int main(int argc, char** argv) {
  auto mandelbrot = tonemap(calculate_mandelbrot(5000, Dimension{256, 256}));
  char const* filename = argc == 2 ? argv[1] : "mandelbrot.bmp";
  std::ofstream outfile(filename, std::ios::binary | std::ios::out);
  bmp::write(outfile, mandelbrot);
}
