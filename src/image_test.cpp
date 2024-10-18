#include "image.h"

#include "test_lib.h"

using namespace image;

static void test_image_throws_if_inconsistent_dimension() {
  TEST_ASSERT_THROWS(([]() {
    image::ImageU8RGB{{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}, {0x0, 0xFF, 0x0}},
                      {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};
  }));
}

static void test_image_copy() {
  image::ImageU8RGB data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                            {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};
  image::ImageU8RGB data2 = data;

  TEST_ASSERT((data2.dimension() == image::Dimension{2, 2}));
  TEST_ASSERT((data2.at(1, 1) == pixel::PixelU8RGB{0x0, 0xFF, 0xFF}));
  TEST_ASSERT((data2.at(Coordinate{.x = 1, .y = 1}) ==
               pixel::PixelU8RGB{0x0, 0xFF, 0xFF}));
}

static void test_index_to_coordinate() {
    image::ImageU8RGB data{Dimension{2, 4}};
    TEST_ASSERT((data.index_to_coord(1) == Coordinate{1, 0}));
    TEST_ASSERT((data.index_to_coord(2) == Coordinate{0, 1}));
    TEST_ASSERT((data.index_to_coord(3) == Coordinate{1, 1}));

    TEST_ASSERT((data.num_pixels() == 2*4));
}

int main(void) {
  test_image_copy();
  test_image_throws_if_inconsistent_dimension();
  test_index_to_coordinate();
}
