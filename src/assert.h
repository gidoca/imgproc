#pragma once

#include <iostream>
#include <sstream>

#define ASSERT(expr, msg)                                                  \
  do {                                                                     \
    if (!(expr)) {                                                         \
      std::stringstream msgstream;                                         \
      msgstream << "ASSERT(" << #expr << ") failed at " << __FILE__ << ":" \
                << __LINE__ << ": " << msg;                                \
      if (print_assert) {                                                  \
        std::cerr << msgstream.str() << std::endl;                         \
      }                                                                    \
      throw msgstream.str();                                               \
    }                                                                      \
  } while (false)

thread_local static bool print_assert{true};

struct disable_print_assert {
  bool old_value;

  disable_print_assert() {
    old_value = print_assert;
    print_assert = false;
  }

  ~disable_print_assert() { print_assert = old_value; }
};
