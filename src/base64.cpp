#include "base64.h"

#include "assert.h"

namespace {
char encode_six_bits(uint8_t byte) {
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

}  // namespace

namespace base64 {
void write(std::span<uint8_t> data, std::ostream& stream) {
  size_t remaining_bits = 0;
  uint8_t remaining_data = 0;

  for (const uint8_t byte : data) {
    ASSERT(uint8_t(remaining_data << remaining_bits) == 0, "");
    uint8_t next_data = (remaining_data | (byte >> remaining_bits)) >> 2;
    stream << encode_six_bits(next_data);
    remaining_data = byte << (6 - remaining_bits);
    remaining_bits += 2;
    if (remaining_bits == 6) {
      stream << encode_six_bits(remaining_data >> 2);
      remaining_bits = 0;
      remaining_data = 0;
    }
  }

  if (remaining_bits == 2) {
    stream << encode_six_bits((remaining_data & 0b11000000) >> 2);
    stream << "==";
  } else if (remaining_bits == 4) {
    stream << encode_six_bits((remaining_data & 0b11110000) >> 2);
    stream << "=";
  } else {
    ASSERT(remaining_bits == 0, "Found unexpected number of remaining bits");
  }
}

}  // namespace base64
