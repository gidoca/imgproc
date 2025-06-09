#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace cmdline {

struct input_arguments {
  int argc;
  char const** argv;
};

struct option_description {
  std::vector<std::string_view> names;
  std::string_view help;
};

struct option_with_arguments {
  option_description description;
  size_t argument_count;

  std::function<void(std::vector<std::string_view>)> found_callback;
};

struct flag {
  option_description description;

  std::function<void()> found_callback;
};

using option = std::variant<option_with_arguments, flag>;

using options = std::vector<option>;

flag make_flag(std::string_view short_name, std::string_view long_name,
               std::string_view help, std::function<void()> found_callback);

option_with_arguments make_option(
    std::string_view short_name, std::string_view long_name,
    std::string_view help, size_t argument_count,
    std::function<void(std::vector<std::string_view>)> found_callback);

bool parse(input_arguments input, options options);

}  // namespace cmdline
