#include "cli.hpp"

#include <iostream>

namespace cli {

bool build_project(std::string_view name) {
  std::cout << "Asked to build : " << name << std::endl;
  return false;
}

} // namespace cli