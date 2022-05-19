#ifndef COMPILER_SHARED_COMMON_HPP
#define COMPILER_SHARED_COMMON_HPP

#include <cstddef>
#include <string>

namespace compiler {
namespace shared {

class location_c {
public:
  location_c() : line(0), col(0) {}
  location_c(std::size_t line, std::size_t col, std::string origin) : line(line), col(col), origin(origin) {}
  std::size_t line{0};
  std::size_t col{0};
  std::string origin;
};

} // namespace shared
} // namespace compiler

#endif