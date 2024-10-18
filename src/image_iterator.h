#pragma once

#include <cstddef>
#include <span>

#include "assert.h"
#include "image.h"

namespace image {

struct PixelIteratorEnd {};

template <typename P>
struct PixelIterator {
  using difference_type = std::ptrdiff_t;

  std::span<P> pixel_buffer;
  std::size_t current_pos;
  difference_type stride;
  std::size_t end_pos;

  bool operator==(PixelIteratorEnd) const { return current_pos == end_pos; }
};

template <typename P>
PixelIterator<P>& operator++(PixelIterator<P>& it) {
  ASSERT(it != PixelIteratorEnd{},
         "Attempting to increment iterator that is already at the end");

  it.current_pos += it.stride;
  return it;
}

template <typename P>
PixelIterator<P> operator++(PixelIterator<P>& it, int) {
  ASSERT(it != PixelIteratorEnd{},
         "Attempting to increment iterator that is already at the end");

  auto old_value = it;
  ++it;
  return old_value;
}

template <typename P>
P const& operator*(PixelIterator<P> const& it) {
  return it.pixel_buffer[it.current_pos];
}

template <typename P>
P& operator*(PixelIterator<P>& it) {
  return it.pixel_buffer[it.current_pos];
}

struct RowIteratorEnd {};

template <typename P>
struct RowIterator {
  using difference_type = std::ptrdiff_t;

  std::span<P> pixel_buffer;
  std::size_t current_pos;
  difference_type row_stride;
  std::size_t row_data_count;
  difference_type column_stride;
  std::size_t end_pos;

  bool operator==(RowIteratorEnd) const { return current_pos == end_pos; }
};

template <typename P>
RowIterator<P>& operator++(RowIterator<P>& it) {
  ASSERT(it != RowIteratorEnd{},
         "Attempting to increment an iterator that is already at the end");

  it.current_pos += it.column_stride;
  return it;
}

template <typename P>
RowIterator<P> operator++(RowIterator<P>& it, int) {
  ASSERT(it != RowIteratorEnd{},
         "Attempting to increment an iterator that is already at the end");

  auto old_value = it;
  ++it;
  return old_value;
}

template <typename P>
RowIterator<P>& operator--(RowIterator<P>& it) {
  ASSERT(
      it.current_pos >= it.column_stride,
      "Attempting to decrement an iterator that is already at the beginning");

  it.current_pos -= it.column_stride;
  return it;
}

template <typename P>
struct RowView {
  using difference_type = std::ptrdiff_t;

  std::span<P> pixel_buffer;
  std::size_t start_pos;
  difference_type stride;
  std::size_t end_pos;

  PixelIterator<P> begin() const {
    return {pixel_buffer, start_pos, stride, end_pos};
  }

  PixelIteratorEnd end() const { return {}; }
};

template <typename P>
RowView<P> operator*(RowIterator<P> const& it) {
  return {it.pixel_buffer, it.current_pos, it.row_stride,
          it.current_pos + it.row_stride * it.row_data_count};
}

template <typename P>
struct ImageView {
  using difference_type = std::ptrdiff_t;

  std::span<P> pixel_buffer;
  std::size_t start_pos;
  difference_type row_stride;
  std::size_t row_data_count;
  difference_type column_stride;
  std::size_t column_data_count;

  RowIterator<const P> cbegin() const {
    return {.pixel_buffer = pixel_buffer,
            .current_pos = start_pos,
            .row_stride = row_stride,
            .row_data_count = row_data_count,
            .column_stride = column_stride,
            .end_pos = start_pos + column_stride * column_data_count};
  }

  RowIteratorEnd cend() const { return {}; }

  RowIterator<P> begin() const {
    return {.pixel_buffer = pixel_buffer,
            .current_pos = start_pos,
            .row_stride = row_stride,
            .row_data_count = row_data_count,
            .column_stride = column_stride,
            .end_pos = start_pos + column_stride * column_data_count};
  }

  RowIteratorEnd end() const { return {}; }

  Dimension dimension() {
    return {.width = row_data_count, .height = column_data_count};
  }
};

template <typename I>
ImageView<typename I::PixelType> view(I& image) {
  return {image.pixels(),
          0,
          1,
          image.width(),
          static_cast<ImageView<typename I::PixelType>::difference_type>(
              image.width()),
          image.height()};
}

template <typename I>
ImageView<const typename I::PixelType> view(I const& image) {
  return {image.pixels(),
          0,
          1,
          image.width(),
          static_cast<ImageView<const typename I::PixelType>::difference_type>(
              image.width()),
          image.height()};
}

template <typename P>
ImageView<P> mirror_horz(ImageView<P> view) {
  return {view.pixel_buffer,
          view.start_pos + (view.row_data_count - 1) * view.row_stride,
          -view.row_stride,
          view.row_data_count,
          view.column_stride,
          view.column_data_count};
}

template <typename P>
Image<P> to_image(ImageView<P> view) {
  std::vector<P> pixels;
  const auto dimension = view.dimension();
  pixels.reserve(dimension.num_elems());

  for (const auto row : view) {
    for (const auto pixel : row) {
      pixels.push_back(pixel);
    }
  }

  return {dimension, std::move(pixels)};
}

}  // namespace image
