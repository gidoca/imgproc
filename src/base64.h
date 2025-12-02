#pragma once

#include <cstddef>
#include <ostream>
#include <span>

#include "assert.h"

namespace base64 {
inline char encode_six_bits(uint8_t byte) {
  if (byte < 26) {
    return 'A' + byte;
  } else if (byte < 52) {
    return 'a' + byte - 26;
  } else if (byte < 62) {
    return '0' + byte - 52;
  } else if (byte == 62) {
    return '+';
  } else if (byte == 63) {
    return '/';
  } else {
    ASSERT(
        false,
        "encode_six_bits() must be called with the top two bits set to zero");
  }
}

template <typename C>
class Writer {
 public:
  Writer(C&& c) : _c(c) {}

  ~Writer() {
    if (remaining_bits == 2) {
      _c(encode_six_bits((remaining_data & 0b11000000) >> 2));
      _c("=");
      _c("=");
    } else if (remaining_bits == 4) {
      _c(encode_six_bits((remaining_data & 0b11110000) >> 2));
      _c("=");
    }
  }

  void write(std::span<std::uint8_t> data) {
    for (const uint8_t byte : data) {
      ASSERT(uint8_t(remaining_data << remaining_bits) == 0, "");
      uint8_t next_data = (remaining_data | (byte >> remaining_bits)) >> 2;
      _c(encode_six_bits(next_data));
      remaining_data = byte << (6 - remaining_bits);
      remaining_bits += 2;
      if (remaining_bits == 6) {
        _c(encode_six_bits(remaining_data >> 2));
        remaining_bits = 0;
        remaining_data = 0;
      }
    }
  }

 private:
  C _c;
  size_t remaining_bits = 0;
  uint8_t remaining_data = 0;
};
}
