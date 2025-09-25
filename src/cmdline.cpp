#include "cmdline.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "assert.h"

namespace cmdline {

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

auto get_description(option const& option) {
  return std::visit([](auto const& option) { return option.description; },
                    option);
}

bool matches_name(std::string_view name, option_description description) {
  return std::ranges::any_of(description.names,
                             [&name](auto const& description_name) {
                               return description_name == name;
                             });
}

struct pop_result {
  input_arguments new_input;
  std::string_view popped_value;
};

std::optional<pop_result> pop(input_arguments input) {
  if (input.argc == 0) {
    return std::nullopt;
  }

  return pop_result{input_arguments{input.argc - 1, input.argv + 1},
                    input.argv[0]};
}

bool has_input(input_arguments input) { return input.argc > 0; }

flag make_flag(std::string_view short_name, std::string_view long_name,
               std::string_view help, std::function<void()> found_callback) {
  return flag{.description = {.names = {short_name, long_name}, .help = help},
              .found_callback = std::move(found_callback)};
}

option_with_arguments make_option(
    std::string_view short_name, std::string_view long_name,
    std::string_view help, size_t argument_count,
    std::function<void(std::vector<std::string_view>)> found_callback) {
  return option_with_arguments{
      .description = {.names = {short_name, long_name}, .help = help},
      .argument_count = argument_count,
      .found_callback = std::move(found_callback)};
}

std::optional<input_arguments> call_callback(input_arguments input,
                                             option option) {
  return std::visit(
      overloaded{[input](option_with_arguments option)
                     -> std::optional<input_arguments> {
                   auto new_input = input;

                   std::vector<std::string_view> arguments;
                   arguments.reserve(option.argument_count);
                   for (size_t i = 0; i < option.argument_count; i++) {
                     const auto pop_result = pop(new_input);
                     if (!pop_result) {
                       return std::nullopt;
                     }
                     arguments.push_back(pop_result->popped_value);
                     new_input = pop_result->new_input;
                   }
                   option.found_callback(arguments);

                   return std::make_optional(new_input);
                 },
                 [input](flag flag) {
                   flag.found_callback();
                   return std::make_optional(input);
                 }},
      option);
}

std::optional<input_arguments> parse_step(input_arguments input,
                                          options options) {
  const auto pop_result = pop(input);

  if (!pop_result) {
    std::cerr << "Error" << std::endl;
    return std::nullopt;
  }
  for (auto const& option : options) {
    if (matches_name(pop_result->popped_value, get_description(option))) {
      return call_callback(pop_result->new_input, option);
    }
  }
  std::cerr << "Unknown option " << pop_result->popped_value << std::endl;
  return std::nullopt;
}

bool parse(const input_arguments input, options options) {
  auto pop_result = pop(input);

  ASSERT(pop_result, "The arguments did not contain the program name");

  auto current_input = pop_result->new_input;

  while (has_input(current_input)) {
    const auto new_input = parse_step(current_input, options);
    if (!new_input) {
      return false;
    }
    current_input = *new_input;
  }
  return true;
}

}  // namespace cmdline
