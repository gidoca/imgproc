#pragma once

#include <concepts>
#include <cstdint>
#include <span>
#include <vector>

namespace image {

template <typename T>
requires std::integral<T> || std::floating_point<T>
struct Pixel {
  T r;
  T g;
  T b;

  bool operator==(Pixel<T> const &other) const = default;
};

using PixelU8 = Pixel<uint8_t>;

struct Dimension {
  size_t width;
  size_t height;

  bool operator==(Dimension const &other) const = default;
};

template <typename T>
class Image {
 public:
  using ElementType = T;
  using PixelType = Pixel<T>;
  using RowType = std::span<PixelType>;
  using ConstRowType = std::span<PixelType const>;

  explicit Image(Dimension dim)
      : _width(dim.width), _data(dim.width * dim.height) {}

  Image(std::initializer_list<std::initializer_list<PixelType>> pixels) {
    if (pixels.size() > 0) {
      _width = begin(pixels)->size();
      auto height = pixels.size();
      _data.reserve(_width * height);
      for (auto const &input_row : pixels) {
        if (input_row.size() != _width) throw "Inconsisten image dimensions";

        _data.insert(end(_data), begin(input_row), end(input_row));
      }
    }
  }

  Image() = default;
  Image(Image const &) = default;
  Image(Image &&) = default;
  Image &operator=(Image const &) = default;
  Image &operator=(Image &&) = default;

  PixelType &at(size_t x, size_t y) { return _data[x + width() * y]; }

  PixelType const &at(size_t x, size_t y) const {
    return _data[x + width() * y];
  }

  RowType row(size_t y) { return RowType{&at(0, y), width()}; }

  ConstRowType row(size_t y) const { return ConstRowType{&at(0, y), width()}; }

  // TODO don't return vector, implement without allocation
  std::vector<ConstRowType> rows() const {
    std::vector<ConstRowType> rows;
    rows.reserve(height());
    for (size_t i = 0; i < height(); i++) {
      rows.push_back(row(i));
    }
    return rows;
  }

  // TODO don't return vector, implement without allocation
  std::vector<RowType> rows() {
    std::vector<RowType> rows;
    rows.reserve(height());
    for (size_t i = 0; i < height(); i++) {
      rows.push_back(row(i));
    }
    return rows;
  }

  size_t width() const { return _width; }

  size_t height() const { return _data.size() / _width; }

  Dimension dimension() const { return {.width = width(), .height = height()}; }

 private:
  std::vector<Pixel<T>> _data;
  size_t _width = 0;
};

using ImageU8 = Image<uint8_t>;

}  // namespace image
