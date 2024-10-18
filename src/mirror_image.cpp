#include <fstream>

#include "bmp.h"
#include "image_iterator.h"

bmp::Image read_image() {
  char const* filename = "testdata/current/mandelbrot.bmp";
  std::ifstream in{filename, std::ios::binary | std::ios::in};

  return bmp::read(in);
}

void write_image(bmp::Image const& image) {
  char const* filename = "testdata/current/mandelbrot_mirrored.bmp";
  std::ofstream out{filename, std::ios::binary | std::ios::out};

  bmp::write(out, image);
}

int main(int argc, char** argv) {
  using image::mirror_horz;
  using image::to_image;
  using image::view;

  auto input_data = read_image();

  auto output_data = to_image(mirror_horz(view(input_data)));

  write_image(output_data);
}
