#pragma once

#include <cstddef>
#include <ostream>
#include <span>

namespace base64 {
void write(std::span<std::uint8_t> data, std::ostream& stream);
}
