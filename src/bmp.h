#pragma once

#include <ostream>

#include "image.h"
#include "pixel.h"

namespace bmp {

using Pixel = pixel::PixelU8RGB;
using Image = image::ImageU8RGB;

void write(std::ostream& stream, Image const& image);

}  // namespace bmp
