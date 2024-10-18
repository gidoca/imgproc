#include "image_iterator.h"

#include <iostream>

#include "image.h"
#include "test_lib.h"

using namespace image;

static void test_image_iterate() {
  const image::ImageU8RGB data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                                  {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};
  size_t y = 0;

  for (auto row : view(data)) {
    size_t x = 0;
    for (auto pixel : row) {
      TEST_ASSERT((data.at(x, y) == pixel));
      x++;
    }
    y++;
  }
}

int main(void) { test_image_iterate(); }
