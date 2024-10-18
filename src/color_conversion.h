#pragma once

#include <algorithm>
#include <cmath>

#include "pixel.h"

namespace color_conversion {

template <typename T>
pixel::PixelSRGB<T> to_srgb(pixel::PixelRGB<T> pixel) {
  pixel::PixelSRGB<T> result;
  std::transform(
      std::begin(pixel.data), std::end(pixel.data), std::begin(result.data),
      [](auto const component) {
        constexpr double gamma = 1 / 2.2;
        double normalized =
            (double)component / (double)pixel::PixelSRGB<T>::unit_value;
        return (T)std::pow(normalized, gamma) * pixel::PixelSRGB<T>::unit_value;
      });
  return result;
}

template <typename T>
pixel::PixelRGB<T> from_srgb(pixel::PixelSRGB<T> pixel) {
  pixel::PixelRGB<T> result;
  std::transform(
      std::begin(pixel.data), std::end(pixel.data), std::begin(result.data),
      [](auto const component) {
        constexpr double gamma = 2.2;
        double normalized =
            (double)component / (double)pixel::PixelSRGB<T>::unit_value;
        return (T)std::pow(normalized, gamma) * pixel::PixelSRGB<T>::unit_value;
      });
  return result;
}

template <typename O, typename I>
O to_underlying(I in_pixel) {
  O result;
  std::transform(std::begin(in_pixel.data), std::end(in_pixel.data),

                 std::begin(result.data), [](auto component) {
                   auto renormalized =
                       component * O::unit_value / I::unit_value;

                   return static_cast<O::ElementType>(renormalized);
                 });
  return result;
}

template <typename T>
pixel::PixelRGB<T> from_hsv(pixel::PixelHSV<T> pixel) {
    T h_prime = (pixel.h() >= 300. ? pixel.h() - 360. : pixel.h()) / 360.;
  auto max = pixel.v();
  auto c = pixel.s() * pixel.v();
  auto min = max - c;

  if (h_prime < 0) {
    return {max, min, min - h_prime * c};
  } else if (h_prime < 1) {
    return {max, min + h_prime * c, min};
  } else if (h_prime < 2) {
    return {min - (h_prime - 2) * c, max, min};
  } else if (h_prime < 3) {
    return {min, max, min + (h_prime - 2) * c};
  } else if (h_prime < 4) {
    return {min, min - (h_prime - 4) * c, max};
  } else {
    return {min + (h_prime - 4) * c, min, max};
  }
}

}  // namespace color_conversion
