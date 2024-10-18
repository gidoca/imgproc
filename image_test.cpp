#include "image.h"

#include <concepts>
#include <exception>
#include <string>
#include <string_view>

#define TEST_ASSERT(cond)                                 \
  do {                                                    \
    test_assert((cond), "Test assertion failed: " #cond); \
  } while (false)
#define TEST_ASSERT_THROWS(callable)                                           \
  do {                                                                         \
    test_assert_throws(                                                        \
        (callable), "Test expected exception, but did not throw: " #callable); \
  } while (false)

class TestFailureException final : public std::exception {
 public:
  TestFailureException(std::string_view msg = "") : message(msg) {}

  const std::string message;

  const char* what() const noexcept override { return message.c_str(); }
};

void test_assert(bool cond, std::string_view msg = "") {
  if (!cond) throw TestFailureException(msg);
}

template <std::invocable C>
void test_assert_throws(C&& c, std::string_view msg = "") {
  bool did_throw = false;
  try {
    c();
  } catch (...) {
    did_throw = true;
  }

  test_assert(did_throw, msg);
}

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
  TEST_ASSERT((data2.at(1, 1) == PixelU8RGB{0x0, 0xFF, 0xFF}));
  TEST_ASSERT(
      (data2.at(Coordinate{.x = 1, .y = 1}) == PixelU8RGB{0x0, 0xFF, 0xFF}));
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
