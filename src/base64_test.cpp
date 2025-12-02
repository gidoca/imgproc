#include "base64.h"

#include <iostream>
#include <sstream>

#include "test_lib.h"

static void test_base64_zero_padding() {
  std::ostringstream output;
  std::array<uint8_t, 6> input{0xf8, 0x00, 0xa2, 0x99, 0x18, 0xff};
  {
    base64::Writer writer([&output](auto c) { output << c; });
    writer.write(input);
  }
  TEST_ASSERT(output.view() == "+ACimRj/");
}

static void test_base64_one_padding() {
  std::ostringstream output;
  std::array<uint8_t, 5> input{0x21, 0x87, 0xd2, 0xba, 0x77};
  {
    base64::Writer writer([&output](auto c) { output << c; });
    writer.write(input);
  }
  TEST_ASSERT(output.view() == "IYfSunc=");
}

static void test_base64_two_padding() {
  std::ostringstream output;
  std::array<uint8_t, 4> input{0x08, 0xc3, 0x1e, 0x1e};
  {
    base64::Writer writer([&output](auto c) { output << c; });
    writer.write(input);
  }
  TEST_ASSERT(output.view() == "CMMeHg==");
}

static void test_base64_empty() {
  std::ostringstream output;
  std::array<uint8_t, 0> input{};
  {
    base64::Writer writer([&output](auto c) { output << c; });
    writer.write(input);
  }
  TEST_ASSERT(output.view() == "");
}

int main() {
  test_base64_zero_padding();
  test_base64_one_padding();
  test_base64_two_padding();
  return EXIT_SUCCESS;
}
