#pragma once

#include <cstddef>

namespace dimension {
struct Dimension {
  std::size_t width;
  std::size_t height;

  bool operator==(Dimension const& other) const = default;

  std::size_t num_elems() const { return width * height; }
};
}  // namespace dimension
