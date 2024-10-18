#include <fstream>
#include <iostream>

#include "bmp.h"
#include "test_lib.h"

static void test_bmp_read() {
  const bmp::Image expected_data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                                    {{0xFF, 0x0, 0xFF}, {0x0, 0xFF, 0xFF}}};
  char const* filename = "testdata/test.bmp";
  std::ifstream in{filename, std::ios::binary | std::ios::in};

  auto input_data = bmp::read(in);

  TEST_ASSERT(expected_data == input_data);
}

int main(void) {
  test_bmp_read();
  return EXIT_SUCCESS;
}
