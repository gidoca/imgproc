set(IMAGE_SRCS bmp.cpp ansi_color.cpp)

add_executable(write_bmp ${IMAGE_SRCS} write_bmp.cpp)
add_executable(write_mandelbrot ${IMAGE_SRCS} mandelbrot.cpp)
add_executable(image_test ${IMAGE_SRCS} image_test.cpp)
add_executable(image_iterator_test ${IMAGE_SRCS} image_iterator_test.cpp)
add_executable(ansi_color_test ${IMAGE_SRCS} ansi_color_test.cpp)
add_executable(ansi_color ${IMAGE_SRCS} ansi_color_main.cpp)
add_executable(read_bmp_test ${IMAGE_SRCS} read_bmp_test.cpp)
add_executable(mirror_image ${IMAGE_SRCS} mirror_image.cpp)
add_executable(cmdline_test cmdline.cpp cmdline_test.cpp)

