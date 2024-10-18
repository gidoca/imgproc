#include "ansi_color.h"

#include <iostream>

namespace ansi_color {

static void print_pixel(Pixel pixel, std::ostream& ostream) {
  ostream << "\033[48;2;" << (unsigned)pixel.r_non_linear() << ";"
          << (unsigned)pixel.g_non_linear() << ";"
          << (unsigned)pixel.b_non_linear() << "m ";
}

static void reset(std::ostream& ostream) { ostream << "\033[0m"; }

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
