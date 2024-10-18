#include <cstdint>
#include <fstream>
#include <iterator>

#include "bit_polyfill.h"
#include "image.h"

using std::begin;
using std::end;

template<typename T>
static void write_basic(std::ofstream& stream, T t) {
  if constexpr (std::endian::native == std::endian::big) {
    t = byteswap(t);
  }
    stream.write(reinterpret_cast<char*>(&t), sizeof(t));
}

constexpr size_t pixel_size = 3;

static void write_pixel(std::ofstream &stream, PixelU8 pixel) {
  write_basic(stream, pixel.r);
  write_basic(stream, pixel.g);
  write_basic(stream, pixel.b);
}

static void write_padding(std::ofstream& stream, size_t n_bytes) {
    for (size_t i = 0; i < n_bytes; i++) {
        write_basic<uint8_t>(stream, 0);
    }
}

static size_t calculate_padding(size_t count) {
    return pixel_size * count % sizeof(uint32_t);
}

template<typename I>
static void write_pixel_row(std::ofstream& stream, I begin, I end) {
    auto count = std::distance(begin, end);
    auto padding = calculate_padding(count);

    for(auto i = begin; i != end; i++) {
        write_pixel(stream, *i);
    }

    write_padding(stream, padding);
}

template<typename T>
static void write_pixel_row(std::ofstream& stream, T const& coll) {
    write_pixel_row(stream, begin(coll), end(coll));
}

constexpr uint32_t data_offset = 54;

static void write_file_header(std::ofstream &stream, uint32_t filesize) {
  constexpr std::string_view header("BM");
  constexpr uint32_t reserved = 0;

  stream << header;
  write_basic(stream, filesize);
  write_basic(stream, reserved);
  write_basic(stream, data_offset);
}

static void write_dib_header(std::ofstream& stream, uint32_t width, uint32_t height) {
    constexpr uint32_t dib_header_size = 40;
    constexpr uint16_t num_planes = 1;
    constexpr uint16_t bpp = 24;
    constexpr uint32_t compression = 0;
    constexpr uint32_t data_size = 16;
    constexpr uint32_t dpi = 2835;
    constexpr uint32_t palette_colors = 0;
    constexpr uint32_t important_colors = 0;
    
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

static uint32_t calculate_file_size(uint32_t width, uint32_t height) {
  return data_offset + (width * pixel_size + calculate_padding(width)) * height;
}

static void write(std::ofstream& stream) {
  const ImageU8 data = {{{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}},
                        {{0xFF, 0x0, 0x0}, {0x0, 0xFF, 0x0}}};
  write_file_header(stream, calculate_file_size(2, 2));
  write_dib_header(stream, 2, 2);
  for (const auto &row : data.rows()) {
    write_pixel_row(stream, row);
  }
    write_padding(stream, 2);
    stream << std::flush;
}

int main(int argc, char** argv) {
    std::ofstream out("file.bmp", std::ios::binary | std::ios::out);
    write(out);
}
