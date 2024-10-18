#pragma once

#include <cstddef>
#include <span>

namespace image {

struct PixelIteratorEnd {};

template <typename P>
struct PixelIterator {
  using difference_type = std::ptrdiff_t;

  std::span<P> pixel_buffer;
  std::size_t current_pos;
  std::size_t stride;
  std::size_t end_pos;

  bool operator==(PixelIteratorEnd) const { return current_pos >= end_pos; }
};

template <typename P>
PixelIterator<P>& operator++(PixelIterator<P>& it) {
  it.current_pos += it.stride;
  return it;
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
  std::size_t row_stride;
  std::size_t row_data_length;
  std::size_t column_stride;
  std::size_t end_pos;

  bool operator==(RowIteratorEnd) const { return current_pos >= end_pos; }
};

template <typename P>
RowIterator<P>& operator++(RowIterator<P>& it) {
  it.current_pos += it.column_stride;
  return it;
}

template <typename P>
RowIterator<P>& operator--(RowIterator<P>& it) {
  it.current_pos -= it.column_stride;
  return it;
}

template <typename P>
struct RowView {
  std::span<P> pixel_buffer;
  std::size_t start_pos;
  std::size_t stride;
  std::size_t end_pos;

  PixelIterator<P> begin() const {
    return {pixel_buffer, start_pos, stride, end_pos};
  }

  PixelIteratorEnd end() const { return {}; }
};

template <typename P>
RowView<P> operator*(RowIterator<P> const& it) {
  return {it.pixel_buffer, it.current_pos, it.row_stride,
          it.current_pos + it.row_data_length};
}

template <typename P>
struct ImageView {
  std::span<P> pixel_buffer;
  std::size_t start_pos;
  std::size_t row_stride;
  std::size_t row_data_length;
  std::size_t column_stride;
  std::size_t column_data_length;

  RowIterator<const P> cbegin() const {
    return {pixel_buffer,    start_pos,     row_stride,
            row_data_length, column_stride, start_pos + column_data_length};
  }

  RowIteratorEnd cend() const { return {}; }

  RowIterator<P> begin() const {
    return {pixel_buffer,    start_pos,     row_stride,
            row_data_length, column_stride, start_pos + column_data_length};
  }

  RowIteratorEnd end() const { return {}; }
};

template <typename I>
ImageView<typename I::PixelType> view(I& image) {
  return {image.pixels(),
          0,
          1,
          image.width(),
          image.width(),
          image.width() * image.height()};
}

template <typename I>
ImageView<const typename I::PixelType> view(I const& image) {
  return {image.pixels(),
          0,
          1,
          image.width(),
          image.width(),
          image.width() * image.height()};
}

}  // namespace image
