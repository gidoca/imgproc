#pragma once

#include <concepts>
#include <exception>
#include <string>
#include <string_view>

#include "assert.h"

#define TEST_ASSERT(cond)                                 \
  do {                                                    \
    test_assert((cond), "Test assertion failed: " #cond); \
  } while (false)

#define TEST_ASSERT_THROWS(callable)                                           \
  do {                                                                         \
    test_assert_throws(                                                        \
        (callable), "Test expected exception, but did not throw: " #callable); \
  } while (false)

#define TEST_FAIL()                            \
  do {                                         \
    test_assert(false, "Unexpectedly called"); \
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
    disable_print_assert d{};
    c();
  } catch (...) {
    did_throw = true;
  }

  test_assert(did_throw, msg);
}
