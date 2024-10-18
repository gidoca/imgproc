#pragma once

namespace pixel {

template <typename T, size_t D>
struct Pixel {
  using ElementType = T;

  static constexpr size_t depth = D;

  T data[D];

  bool operator==(Pixel<T, depth> const& other) const = default;
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

using PixelU8RGB = PixelRGB<uint8_t>;

}  // namespace pixel
