#include "page.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace compiler {
namespace shared {

bool page_c::load_page(const std::string &file) {

  _file = file;
  if (!std::filesystem::is_regular_file(_file)) {
    return false;
  }

  std::fstream inf;
  inf.open(_file, std::fstream::in);
  if (!inf.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(inf, line)) {
    _data.push_back(line);
  }
  return true;
}

std::tuple<bool, std::string> page_c::get_line(std::size_t line) {
  if (line >= _data.size()) {
    return {false, {}};
  }
  return {true, _data.at(line)};
}

} // namespace shared
} // namespace compiler