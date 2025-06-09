#include "cmdline.h"

#include "test_lib.h"

namespace cmdline {

void test_empty_arguments() {
  char const* argv[] = {"cmdline_test"};
  input_arguments input{1, argv};

  options options{make_flag("-f", "--flag", "Help", []() { TEST_FAIL(); }),
                  make_option("-o", "--option", "Help", 1,
                              [](auto const& values) { TEST_FAIL(); })};

  TEST_ASSERT(parse(input, options));
}

void test_flag() {
  char const* argv[] = {"cmdline_test", "-f"};
  input_arguments input{2, argv};

  size_t flag_instances{0};

  options options{make_flag("-f", "--flag", "Help",
                            [&flag_instances]() { flag_instances++; }),
                  make_option("-o", "--option", "Help", 1,
                              [](auto const& values) { TEST_FAIL(); })};

  TEST_ASSERT(parse(input, options));

  TEST_ASSERT(flag_instances == 1);
}

}  // namespace cmdline

int main(int argc, char** argv) {
  cmdline::test_empty_arguments();
  cmdline::test_flag();
  return 0;
}
