#include <complex>
#include <fstream>

#include "bmp.h"
#include "colors.h"
#include "image.h"

using image::Coordinate;
using image::Dimension;
using image::Image;
using pixel::Pixel;

using complex = std::complex<float>;

using ComplexImage = Image<Pixel<complex, 1>>;

static constexpr float max = 2.f;
static constexpr float max_absolute_iter_value = 2.f;
static constexpr float max_norm_iter_value =
    max_absolute_iter_value * max_absolute_iter_value;

bmp::Image tonemap(ComplexImage const& im) {
  bmp::Image out{im.dimension()};
  auto const& in_pixels = im.pixels();
  auto& out_pixels = out.pixels();
  for (size_t i = 0; i < in_pixels.size(); ++i) {
    complex in_pixel = in_pixels[i];
    out_pixels[i] = norm(in_pixel) > max_norm_iter_value ? colors::u8::black
                                                         : colors::u8::white;
  }
  return out;
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

void calculate_iteration(ComplexImage& image) {
  image.map_inplace(
      [](complex& pixel, Coordinate coord, ComplexImage const& image) {
        if (norm(pixel) <=
            max_norm_iter_value +
                std::numeric_limits<complex::value_type>::epsilon()) {
          auto c = coord_to_complex(coord, image.dimension());
          pixel = pixel * pixel + c;
        }
      });
}

ComplexImage calculate_mandelbrot(size_t num_iterations, Dimension dim) {
  ComplexImage out{dim};
  for (size_t iteration = 0; iteration < num_iterations; ++iteration) {
    calculate_iteration(out);
  }
  return out;
}

int main(int argc, char** argv) {
  auto mandelbrot = tonemap(calculate_mandelbrot(300, Dimension{64, 64}));
  char const* filename = argc == 2 ? argv[1] : "mandelbrot.bmp";
  std::ofstream outfile(filename, std::ios::binary | std::ios::out);
  bmp::write(outfile, mandelbrot);
}
