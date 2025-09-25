#include "ppm.h"

namespace ppm {

void write(std::ostream& stream, Image const& image) {
    stream << "P3\n"
           << image.width() << " " << image.height() << "\n"
           << "255\n";
    for (auto const& pixel : image.pixels()) {
        stream << (unsigned)pixel.r_non_linear() << " "
               << (unsigned)pixel.g_non_linear() << " "
               << (unsigned)pixel.b_non_linear() << "\n";
    }
}

Image read(std::istream& stream) {
    // TODO: Implement PPM reading
    return Image{};
}

}  // namespace ppm