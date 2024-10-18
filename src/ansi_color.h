#pragma once

#include <ostream>

#include "image.h"
#include "image_iterator.h"
#include "pixel.h"

namespace ansi_color {
using Pixel = pixel::PixelU8SRGB;
using Image = image::ImageU8SRGB;

void print_pixel(Pixel pixel, std::ostream& ostream) {
  ostream << "\033[48;2;" << (unsigned)pixel.r_non_linear() << ";"
          << (unsigned)pixel.g_non_linear() << ";"
          << (unsigned)pixel.b_non_linear() << "m \033[0m";
}

void reset(std::ostream& ostream) { ostream << "\033[0m"; }

void print_image(Image const& image, std::ostream& ostream) {
  reset(ostream);

  for (auto row : mirror_vert(view(image))) {
    for (auto pixel : row) {
      print_pixel(pixel, ostream);
    }
    ostream << "\n";
  }

  reset(ostream);
  ostream.flush();
}
}  // namespace ansi_color
