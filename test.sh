set -e

clang++ -std=c++20 -stdlib=libc++ -Wall write_bmp.cpp -o bin/write_bmp
./bin/write_bmp
diff file.bmp ref.bmp

clang++ -std=c++20 -stdlib=libc++ -Wall mandelbrot.cpp -o bin/write_mandelbrot
./bin/write_mandelbrot
diff mandelbrot.bmp mandelbrotref.bmp

clang++ -std=c++20 -stdlib=libc++ -Wall image_test.cpp -o bin/image_test
./bin/image_test

