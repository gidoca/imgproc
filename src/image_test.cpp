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

  TEST_ASSERT((data.num_pixels() == 2 * 4));
}

static void test_map_pixels() {
  image::ImageU8RGB data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                            {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};

  image::ImageU16RGB mapped = data.map_pixels([](pixel::PixelU8RGB pixel) {
    return pixel::PixelU16RGB{static_cast<uint16_t>(pixel.r() * 2),
                              static_cast<uint16_t>(pixel.g() * 3),
                              static_cast<uint16_t>(pixel.b() * 4)};
  });

  image::ImageU16RGB expected_output = {
      {{2 * 0xFF, 0x0, 0x0}, {0x0, 3 * 0xFF, 0x0}},
      {{2 * 0xFF, 0x0, 4 * 0xff}, {0x0, 3 * 0xFF, 4 * 0xFF}}};

  TEST_ASSERT(mapped == expected_output);
}

int main(void) {
  test_image_copy();
  test_image_throws_if_inconsistent_dimension();
  test_index_to_coordinate();
}
