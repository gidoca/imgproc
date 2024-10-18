#include <fstream>

#include "bmp.h"

int main(int argc, char** argv) {
  const bmp::Image data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                           {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};
  std::ofstream out("file.bmp", std::ios::binary | std::ios::out);
  bmp::write(out, data);
  return EXIT_SUCCESS;
}
