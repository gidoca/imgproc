#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <iterator>
#include <span>
#include <vector>

#include "assert.h"
#include "pixel.h"

namespace image {

struct Dimension {
  size_t width;
  size_t height;

  bool operator==(Dimension const& other) const = default;

  size_t num_elems() const { return width * height; }
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

  explicit Image(Dimension dim) : _data(dim.num_elems()), _width(dim.width) {}

  Image(Dimension dim, std::vector<PixelType> data)
      : _data(std::move(data)), _width(dim.width) {
    ASSERT(dim.num_elems() == data.size(), "Expected " << dim.num_elems()
                                                       << " elements but got "
                                                       << data.size());
  }

  Image(std::initializer_list<std::initializer_list<PixelType>> pixels) {
    if (pixels.size() > 0) {
      _width = begin(pixels)->size();
      auto height = pixels.size();
      _data.reserve(_width * height);
      for (auto const& input_row : pixels) {
        ASSERT((input_row.size() == _width),
               "Inconsistent image dimensions, previous row had width "
                   << _width << " but got a row with width "
                   << input_row.size());

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

  std::vector<PixelType>& pixels() { return _data; }
  std::vector<PixelType> const& pixels() const { return _data; }

  size_t width() const { return _width; }

  size_t height() const { return _data.size() / _width; }

  size_t num_pixels() const { return _data.size(); }

  Dimension dimension() const { return {.width = width(), .height = height()}; }

  Coordinate index_to_coord(size_t i) const { return {.x = i % _width, .y = i / _width}; }

  auto map_pixels(auto callable) {
    using OutPixelType = decltype(callable(PixelType{}));

    std::vector<OutPixelType> out_pixels;
    out_pixels.reserve(num_pixels());
    std::transform(_data.begin(), _data.end(), std::back_inserter(out_pixels), callable);
    return Image<OutPixelType>{dimension(), std::move(out_pixels)};
  }

  void map_inplace(auto callable) {
    size_t index = 0;
    for (auto& pixel : pixels()) {
      callable(pixel, index_to_coord(index), const_cast<Image const&>(*this));
      index++;
    }
  }

  bool operator==(Image<PixelType> const& other) const = default;

 private:
  std::vector<PixelType> _data;
  size_t _width = 0;
};

using ImageU8RGB = Image<pixel::PixelU8RGB>;
using ImageU8SRGB = Image<pixel::PixelU8SRGB>;
using ImageU16RGB = Image<pixel::PixelU16RGB>;
using ImageU16SRGB = Image<pixel::PixelU16SRGB>;
using ImageF32RGB = Image<pixel::PixelF32SRGB>;
using ImageF32SRGB = Image<pixel::PixelF32SRGB>;

}  // namespace image
