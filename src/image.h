#pragma once

#include <concepts>
#include <cstdint>
#include <span>
#include <vector>

#include "pixel.h"

namespace image {

struct Dimension {
  size_t width;
  size_t height;

  bool operator==(Dimension const& other) const = default;
};

struct Coordinate {
    size_t x;
    size_t y;

    bool operator==(Coordinate const& other) const = default;
};

template <typename P>
class Image {
 public:
  using PixelType = P;
  using ElementType = typename PixelType::ElementType;
  using RowType = std::span<PixelType>;
  using ConstRowType = std::span<PixelType const>;

  explicit Image(Dimension dim)
      :  _data(dim.width * dim.height), _width(dim.width) {}

  Image(std::initializer_list<std::initializer_list<PixelType>> pixels) {
    if (pixels.size() > 0) {
      _width = begin(pixels)->size();
      auto height = pixels.size();
      _data.reserve(_width * height);
      for (auto const& input_row : pixels) {
        if (input_row.size() != _width) throw "Inconsistent image dimensions";

        _data.insert(end(_data), begin(input_row), end(input_row));
      }
    }
  }

  Image() = default;
  Image(Image const&) = default;
  Image(Image&&) = default;
  Image& operator=(Image const&) = default;
  Image& operator=(Image&&) = default;

  PixelType& at(size_t x, size_t y) { return _data[x + width() * y]; }
  PixelType const& at(size_t x, size_t y) const {
    return _data[x + width() * y];
  }

  PixelType& at(Coordinate coord) { return at(coord.x, coord.y); }
  PixelType const& at(Coordinate coord) const { return at(coord.x, coord.y); }

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

  std::vector<PixelType>& pixels() { return _data; }
  std::vector<PixelType> const& pixels() const { return _data; }

  size_t width() const { return _width; }

  size_t height() const { return _data.size() / _width; }

  size_t num_pixels() const { return _data.size(); }

  Dimension dimension() const { return {.width = width(), .height = height()}; }

  Coordinate index_to_coord(size_t i) const { return {.x = i % _width, .y = i / _width}; }

  void map_inplace(auto callable) {
    size_t index = 0;
    for (auto& pixel : pixels()) {
      callable(pixel, index_to_coord(index), const_cast<Image const&>(*this));
      index++;
    }
  }

 private:
  std::vector<PixelType> _data;
  size_t _width = 0;
};

using ImageU8RGB = Image<pixel::PixelU8RGB>;
using ImageU8SRGB = Image<pixel::PixelU8SRGB>;
using ImageF32RGB = Image<pixel::PixelF32SRGB>;
using ImageF32SRGB = Image<pixel::PixelF32SRGB>;

}  // namespace image
