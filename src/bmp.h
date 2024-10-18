#pragma once

#include <ostream>

#include "image.h"
#include "pixel.h"

namespace bmp {

using Pixel = pixel::PixelU8SRGB;
using Image = image::ImageU8SRGB;

void write(std::ostream& stream, Image const& image);

}  // namespace bmp
