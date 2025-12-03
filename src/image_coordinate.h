#pragma once

#include <cstddef>

namespace image {

struct Coordinate {
  std::size_t x;
  std::size_t y;

  bool operator==(Coordinate const& other) const = default;
};

}  // namespace image
