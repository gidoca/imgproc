#include "ansi_color.h"

#include <iostream>

static void print_image() {
  ansi_color::Image data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                            {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};

  ansi_color::print_image(data, std::cout);
}

int main() { print_image(); }
