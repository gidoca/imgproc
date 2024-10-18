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

}  // namespace color_conversion
