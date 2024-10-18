#pragma once

#include <iostream>
#include <sstream>

#define ASSERT(expr, msg)                                                  \
  do {                                                                     \
    if (!(expr)) {                                                         \
      std::stringstream msgstream;                                         \
      msgstream << "ASSERT(" << #expr << ") failed at " << __FILE__ << ":" \
                << __LINE__ << ": " << msg;                                \
      std::cerr << msgstream.str() << std::endl;                           \
      throw msgstream.str();                                               \
    }                                                                      \
  } while (false)
