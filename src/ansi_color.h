#pragma once

#include <ostream>

#include "image.h"
#include "image_iterator.h"
#include "pixel.h"

namespace ansi_color {
using Pixel = pixel::PixelU8SRGB;
using Image = image::ImageU8SRGB;

static constexpr char reset[] = "\033[0m";
static constexpr char red[] = "\033[31m";
static constexpr char green[] = "\033[32m";
static constexpr char yellow[] = "\033[33m";
static constexpr char blue[] = "\033[34m";
static constexpr char magenta[] = "\033[35m";
static constexpr char cyan[] = "\033[36m";
static constexpr char white[] = "\033[37m";
static constexpr char bold[] = "\033[1m";
static constexpr char underline[] = "\033[4m";

void print_image(Image const& image, std::ostream& ostream);
}  // namespace ansi_color
