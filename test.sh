set -e

clang++ -std=c++20 -stdlib=libc++ write_bmp.cpp
./a.out
diff file.bmp ref.bmp
