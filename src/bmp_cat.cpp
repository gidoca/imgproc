#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ansi_color.h"
#include "bmp.h"
#include "cmdline.h"

std::vector<bmp::Image> read(std::vector<std::string> const& files) {
  if (files.empty()) {
    return {bmp::read(std::cin)};
  } else {
    std::vector<bmp::Image> images;
    for (auto const& file : files) {
      std::ifstream input{file, std::ios::binary};
      if (!input) {
        throw std::runtime_error("Could not open file " + file);
      }
      images.push_back(bmp::read(input));
    }
    return images;
  }
}

int main(int argc, const char** argv) {
  std::vector<std::string> files;
  if (!cmdline::parse(
          {argc, argv},
          {
              cmdline::make_flag(
                  "-h", "--help", "Show help",
                  [] {
                    std::cout
                        << "Usage: bmp_cat [FILE]...\n"
                        << "Print BMP files as ANSI color to standard output.\n"
                        << "\n"
                        << "If no FILE is given, read from standard input."
                        << std::endl;
                    exit(0);
                  }),
              cmdline::make_option(
                  "-f", "--file",
                  "Input BMP file (can be specified multiple times)", 1,
                  [&files](std::vector<std::string_view> values) {
                    files.push_back(std::string(values[0]));
                  }),
          })) {
    std::cerr << ansi_color::red << "Error: Could not parse command line"
              << ansi_color::reset << std::endl;
    return 1;
  }

  try {
    auto images = read(files);
    for (auto const& image : images) {
      ansi_color::print_image(image, std::cout);
    }
  } catch (std::exception const& e) {
    std::cerr << ansi_color::red << "Error: " << e.what() << ansi_color::reset
              << std::endl;
    return 1;
  }

  return 0;
}