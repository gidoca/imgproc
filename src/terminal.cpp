#include "terminal.h"

#include <sys/ioctl.h>
#include <unistd.h>

namespace terminal {
Dimension pixel_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return {w.ws_xpixel, w.ws_ypixel};
}
Dimension char_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return {w.ws_row, w.ws_col};
}
}  // namespace terminal
