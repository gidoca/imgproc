#include <fstream>
#include <iostream>

#include "ansi_color.h"
#include "bmp.h"

int main() {
  char const* filename = "testdata/mandelbrot.bmp";
  std::ifstream in{filename, std::ios::binary | std::ios::in};

  auto bmp_image = bmp::read(in);
  using std::cout;
  ansi_color::print_image(bmp_image, cout);
}
