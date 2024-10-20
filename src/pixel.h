#pragma once

#include <cstdint>

namespace pixel {

template <typename T>
struct unit_value {
  static constexpr T value{};
};

template <>
struct unit_value<uint8_t> {
  static constexpr uint8_t value = 255;
};

template <>
struct unit_value<float> {
  static constexpr float value = 1.f;
};

template <typename T, size_t D>
struct Pixel {
  using ElementType = T;

  static constexpr size_t depth = D;
  static constexpr T unit_value = pixel::unit_value<T>::value;

  T data[D];

  bool operator==(Pixel<T, D> const& other) const = default;
};

template <typename T>
struct Pixel<T, 1> {
  using ElementType = T;

  Pixel() = default;
  Pixel(Pixel const& other) = default;
  Pixel(Pixel&& other) = default;
  Pixel& operator=(Pixel const& other) = default;
  Pixel& operator=(Pixel&& other) = default;
  Pixel(T const& element) { data[0] = element; }
  Pixel(T&& element) { data[0] = std::move(element); }

  static constexpr size_t depth = 1;
  static constexpr T unit_value = pixel::unit_value<T>::value;

  T data[1];

  bool operator==(Pixel<T, depth> const& other) const = default;
  operator T const&() const { return data[0]; }
  operator T&() { return data[0]; }
};

template <typename T>
struct PixelRGB : Pixel<T, 3> {
  T& r() { return this->data[0]; }
  T& g() { return this->data[1]; }
  T& b() { return this->data[2]; }
  T const& r() const { return this->data[0]; }
  T const& g() const { return this->data[1]; }
  T const& b() const { return this->data[2]; }
};

template <typename T>
struct PixelSRGB : Pixel<T, 3> {
  T& r_non_linear() { return this->data[0]; }
  T& g_non_linear() { return this->data[1]; }
  T& b_non_linear() { return this->data[2]; }
  T const& r_non_linear() const { return this->data[0]; }
  T const& g_non_linear() const { return this->data[1]; }
  T const& b_non_linear() const { return this->data[2]; }
};

template <typename T>
struct PixelHSV : Pixel<T, 3> {
  T& h() { return this->data[0]; }
  T& s() { return this->data[1]; }
  T& v() { return this->data[2]; }
  T const& h() const { return this->data[0]; }
  T const& s() const { return this->data[1]; }
  T const& v() const { return this->data[2]; }
};

using PixelU8RGB = PixelRGB<uint8_t>;
using PixelU8SRGB = PixelSRGB<uint8_t>;
using PixelU8HSV = PixelHSV<uint8_t>;
using PixelU16RGB = PixelRGB<uint16_t>;
using PixelU16SRGB = PixelSRGB<uint16_t>;
using PixelU16HSV = PixelHSV<uint16_t>;
using PixelF32RGB = PixelRGB<float>;
using PixelF32SRGB = PixelSRGB<float>;
using PixelF32HSV = PixelHSV<float>;

}  // namespace pixel
