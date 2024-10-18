set(IMAGE_SRCS bmp.cpp)

add_executable(write_bmp ${IMAGE_SRCS} write_bmp.cpp)
add_executable(write_mandelbrot ${IMAGE_SRCS} mandelbrot.cpp)
add_executable(image_test ${IMAGE_SRCS} image_test.cpp)
add_executable(image_iterator_test ${IMAGE_SRCS} image_iterator_test.cpp)
