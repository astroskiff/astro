#ifndef COMPILER_SHARED_COMMON_HPP
#define COMPILER_SHARED_COMMON_HPP

#include <cstddef>

namespace compiler {
namespace shared {

class location_c {
public:
  location_c() : line(0), col(0) {}
  location_c(std::size_t line, std::size_t col) : line(line), col(col) {}
  std::size_t line{0};
  std::size_t col{0};
};

} // namespace shared
} // namespace compiler

#endif