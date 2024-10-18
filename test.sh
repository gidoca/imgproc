set -e

clang++ -std=c++20 -stdlib=libc++ -Wall write_bmp.cpp -o write_bmp
./write_bmp
diff file.bmp ref.bmp

clang++ -std=c++20 -stdlib=libc++ -Wall image_test.cpp -o image_test
./image_test

