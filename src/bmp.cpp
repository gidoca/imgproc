#include "bmp.h"

#include <bit>
#include <cstdint>
#include <iterator>
#include <ostream>

#include "image.h"
#include "image_iterator.h"

namespace bmp {

template <typename T>
static void write_basic(std::ostream& stream, T t) {
  if constexpr (std::endian::native == std::endian::big) {
    t = byteswap(t);
  }
  stream.write(std::bit_cast<std::array<char, sizeof(T)>>(t).data(), sizeof(t));
}

constexpr size_t pixel_size = 3;

static void write_pixel(std::ostream& stream, Pixel pixel) {
  write_basic(stream, pixel.r_non_linear());
  write_basic(stream, pixel.g_non_linear());
  write_basic(stream, pixel.b_non_linear());
}

static void write_padding(std::ostream& stream, size_t n_bytes) {
  for (size_t i = 0; i < n_bytes; i++) {
    write_basic<uint8_t>(stream, 0);
  }
}

static size_t calculate_padding(size_t count) {
  return pixel_size * count % sizeof(uint32_t);
}

constexpr uint32_t data_offset = 54;

static uint32_t calculate_file_size(image::Dimension dimension) {
  return data_offset +
         (dimension.width * pixel_size + calculate_padding(dimension.width)) *
             dimension.height;
}

template <typename B, typename E>
static void write_pixel_row(std::ostream& stream, B begin, E end) {
  auto count = std::ranges::distance(begin, end);
  auto padding = calculate_padding(count);

  for (auto i = begin; i != end; i++) {
    write_pixel(stream, *i);
  }

  write_padding(stream, padding);
}

template <typename T>
static void write_pixel_row(std::ostream& stream, T const& coll) {
  write_pixel_row(stream, coll.begin(), coll.end());
}

static void write_file_header(std::ostream& stream,
                              image::Dimension dimension) {
  constexpr std::string_view header("BM");
  constexpr uint32_t reserved = 0;
  const uint32_t filesize = calculate_file_size(dimension);

  stream << header;
  write_basic(stream, filesize);
  write_basic(stream, reserved);
  write_basic(stream, data_offset);
}

static void write_dib_header(std::ostream& stream, image::Dimension dimension) {
  constexpr uint32_t dib_header_size = 40;
  constexpr uint16_t num_planes = 1;
  constexpr uint16_t bpp = 24;
  constexpr uint32_t compression = 0;
  constexpr uint32_t data_size = 16;
  constexpr uint32_t dpi = 2835;
  constexpr uint32_t palette_colors = 0;
  constexpr uint32_t important_colors = 0;

  const uint32_t width = dimension.width;
  const uint32_t height = dimension.height;

  write_basic(stream, dib_header_size);
  write_basic(stream, width);
  write_basic(stream, height);
  write_basic(stream, num_planes);
  write_basic(stream, bpp);
  write_basic(stream, compression);
  write_basic(stream, data_size);
  write_basic(stream, dpi);
  write_basic(stream, dpi);
  write_basic(stream, palette_colors);
  write_basic(stream, important_colors);
}

void write(std::ostream& stream, Image const& image) {
  write_file_header(stream, image.dimension());
  write_dib_header(stream, image.dimension());
  for (const auto& row : view(image)) {
    write_pixel_row(stream, row);
  }
  stream << std::flush;
}

}  // namespace bmp
