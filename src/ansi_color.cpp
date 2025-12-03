#include "ansi_color.h"

#include <iostream>

#include "base64.h"

namespace ansi_color {

static void print_pixel(Pixel pixel, std::ostream& ostream) {
  ostream << "\033[48;2;" << (unsigned)pixel.r_non_linear() << ";"
          << (unsigned)pixel.g_non_linear() << ";"
          << (unsigned)pixel.b_non_linear() << "m ";
}

static void reset_term(std::ostream& ostream) { ostream << ansi_color::reset; }

/*void print_image(Image const& image, std::ostream& ostream) {
  reset_term(ostream);

  for (auto row : mirror_vert(view(image))) {
    for (auto pixel : row) {
      print_pixel(pixel, ostream);
    }
    ostream << ansi_color::reset << "\n";
  }

  reset_term(ostream);
  ostream.flush();
}*/

void print_image(Image const& image, std::ostream& ostream) {
  reset_term(ostream);

  ostream << "\e_G" << "a=T,f=24," << "s=" << image.width()
          << ",v=" << image.height() << ";";

  {
    base64::Writer writer{[&ostream](auto c) { ostream << c; }};
    for (auto row : mirror_vert(view(image))) {
      for (auto pixel : row) {
        writer.write(pixel.data);
      }
    }
  }

  ostream << "\e\\";

  reset_term(ostream);
  ostream.flush();
}

}  // namespace ansi_color
