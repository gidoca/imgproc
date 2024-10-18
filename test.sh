set -e

clang++ -std=c++20 -stdlib=libc++ -Wall write_bmp.cpp
./a.out
diff file.bmp ref.bmp
