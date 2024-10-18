#pragma once

#include <ostream>

#include "image.h"
#include "image_iterator.h"
#include "pixel.h"

namespace ansi_color {
using Pixel = pixel::PixelU8SRGB;
using Image = image::ImageU8SRGB;

void print_image(Image const& image, std::ostream& ostream);
}  // namespace ansi_color
