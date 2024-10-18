#pragma once

#include <algorithm>
#include <bit>
#include <version>

namespace std {

#if __cpp_lib_bit_cast < 201806
template <class To, class From>
std::enable_if_t<sizeof(To) == sizeof(From) &&
                     std::is_trivially_copyable_v<From> &&
                     std::is_trivially_copyable_v<To>,
                 To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept {
  static_assert(std::is_trivially_constructible_v<To>,
                "This implementation additionally requires "
                "destination type to be trivially constructible");

  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}
#endif

// Polyfill, since std::byteswap is not yet available
#if __cpp_lib_byteswap < 202110
template <std::integral T>
constexpr T byteswap(T value) noexcept {
  static_assert(std::has_unique_object_representations_v<T>,
                "T may not have padding bits");
  auto value_representation =
      std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
  std::reverse(begin(value_representation), end(value_representation));
  return std::bit_cast<T>(value_representation);
}
#endif

}  // namespace std
