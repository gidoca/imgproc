#include "bmp.h"

#include <bit>
#include <cstdint>
#include <istream>
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

template <typename T>
static void read_basic(std::istream& stream, T& t) {
  std::array<char, sizeof(T)> data;
  stream.read(data.data(), sizeof(t));
  t = std::bit_cast<T>(data);
  if constexpr (std::endian::native == std::endian::big) {
    t = byteswap(t);
  }
}

constexpr size_t pixel_size = 3;

static void write_pixel(std::ostream& stream, Pixel pixel) {
  write_basic(stream, pixel.b_non_linear());
  write_basic(stream, pixel.g_non_linear());
  write_basic(stream, pixel.r_non_linear());
}

static Pixel read_pixel(std::istream& stream) {
  Pixel pixel;

  read_basic(stream, pixel.b_non_linear());
  read_basic(stream, pixel.g_non_linear());
  read_basic(stream, pixel.r_non_linear());

  return pixel;
}

static void write_padding(std::ostream& stream, size_t n_bytes) {
  for (size_t i = 0; i < n_bytes; i++) {
    write_basic<uint8_t>(stream, 0);
  }
}

static void skip_padding(std::istream& stream, size_t n_bytes) {
  stream.seekg(n_bytes, std::ios_base::cur);
}

static size_t calculate_padding(size_t count) {
  return pixel_size * count % sizeof(uint32_t);
}

constexpr uint32_t data_offset = 54;

static uint32_t calculate_data_size(image::Dimension dimension) {
  return (dimension.width * pixel_size + calculate_padding(dimension.width)) *
         dimension.height;
}

static uint32_t calculate_file_size(image::Dimension dimension) {
  return data_offset + calculate_data_size(dimension);
}

template <typename P>
static void write_pixel_row(std::ostream& stream,
                            image::RowView<const P> row_view) {
  auto count = std::ranges::distance(row_view.begin(), row_view.end());
  auto padding = calculate_padding(count);

  for (auto i = row_view.begin(); i != row_view.end(); i++) {
    write_pixel(stream, *i);
  }

  write_padding(stream, padding);
}

template <typename P>
static void read_pixel_row(std::istream& stream, image::RowView<P> row_view) {
  auto count = std::ranges::distance(row_view.begin(), row_view.end());
  auto padding = calculate_padding(count);

  for (auto i = row_view.begin(); i != row_view.end(); i++) {
    *i = read_pixel(stream);
  }

  skip_padding(stream, padding);
}

template <typename T>
static void write_pixel_row(std::ostream& stream, T const& coll) {
  write_pixel_row(stream, coll.begin(), coll.end());
}

struct FileHeader final {
  uint32_t filesize;
  uint32_t reserved = 0;
  uint32_t data_offset;
};

struct DibHeader final {
  uint32_t dib_header_size = 40;
  uint32_t width;
  uint32_t height;
  uint16_t num_planes = 1;
  uint16_t bpp = 24;
  uint32_t compression = 0;
  uint32_t data_size;
  uint32_t dpi = 2835;
  uint32_t palette_colors = 0;
  uint32_t important_colors = 0;
};

static DibHeader dib_header_for_dimension(image::Dimension dimension) {
  return DibHeader{.width = static_cast<uint32_t>(dimension.width),
                   .height = static_cast<uint32_t>(dimension.height),
                   .data_size = calculate_data_size(dimension)};
}

static FileHeader read_file_header(std::istream& stream) {
  {
    char c{};
    stream >> c;
    ASSERT(c == 'B', "Expected magic values to be \"B\" but got " << c);
    stream >> c;
    ASSERT(c == 'M', "Expected magic values to be \"M\" but got " << c);
  }

  FileHeader file_header;

  read_basic(stream, file_header.filesize);
  read_basic(stream, file_header.reserved);
  read_basic(stream, file_header.data_offset);

  return file_header;
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

static DibHeader read_dib_header(std::istream& stream) {
  DibHeader dib_header{};

  read_basic(stream, dib_header.dib_header_size);
  read_basic(stream, dib_header.width);
  read_basic(stream, dib_header.height);
  read_basic(stream, dib_header.num_planes);
  read_basic(stream, dib_header.bpp);
  read_basic(stream, dib_header.compression);
  read_basic(stream, dib_header.data_size);
  read_basic(stream, dib_header.dpi);
  read_basic(stream, dib_header.dpi);
  read_basic(stream, dib_header.palette_colors);
  read_basic(stream, dib_header.important_colors);

  return dib_header;
}

static void write_dib_header(std::ostream& stream, image::Dimension dimension) {
  const auto dib_header{dib_header_for_dimension(dimension)};

  write_basic(stream, dib_header.dib_header_size);
  write_basic(stream, dib_header.width);
  write_basic(stream, dib_header.height);
  write_basic(stream, dib_header.num_planes);
  write_basic(stream, dib_header.bpp);
  write_basic(stream, dib_header.compression);
  write_basic(stream, dib_header.data_size);
  write_basic(stream, dib_header.dpi);
  write_basic(stream, dib_header.dpi);
  write_basic(stream, dib_header.palette_colors);
  write_basic(stream, dib_header.important_colors);
}

static Image read_pixel_array(std::istream& stream, DibHeader dib_header) {
  Image image{
      image::Dimension{.width = dib_header.width, .height = dib_header.height}};

  for (auto row : view(image)) {
    read_pixel_row(stream, row);
  }

  return image;
}

static void write_pixel_array(std::ostream& stream, Image const& image) {
  for (auto row : view(image)) {
    write_pixel_row(stream, row);
  }
}

void write(std::ostream& stream, Image const& image) {
  write_file_header(stream, image.dimension());
  write_dib_header(stream, image.dimension());
  write_pixel_array(stream, image);
  stream << std::flush;
}

Image read(std::istream& stream) {
  auto file_header = read_file_header(stream);
  auto dib_header = read_dib_header(stream);

  stream.seekg(file_header.data_offset);

  return read_pixel_array(stream, dib_header);
}

}  // namespace bmp
