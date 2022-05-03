#ifndef COMPILER_LANG_COMMON_HPP
#define COMPILER_LANG_COMMON_HPP

#include "compiler/source/page.hpp"
#include "compiler/context/context.hpp"

#include <filesystem>
#include <stack>
#include <string>
#include <unordered_map>

namespace compiler {
namespace lang {

//! \brief Data type passed around to various pieces of the compiler
//!        to aggregate data required for compiling and error reporting
struct compiler_data_t {
  std::stack<std::filesystem::path>
      operating_directory; // Indicate what directory is being operated in
  std::unordered_map<std::string, compiler::source::page_c>
      source_code; // Source code for project
  std::unordered_map<std::string, context::context_c>
      contexts; // Modules + Application scope
  std::vector<std::string> known_modules;
};

} // namespace lang
} // namespace compiler

#endif