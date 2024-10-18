cmake_minimum_required(VERSION 3.24)

project(ImageProc)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(src)

install(TARGETS write_bmp write_mandelbrot image_test image_iterator_test ansi_color_test ansi_color read_bmp_test mirror_image DESTINATION bin)
