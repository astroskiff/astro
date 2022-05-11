#include <filesystem>
#include <iostream>

#include "compiler/compiler.hpp"

int main(int argc, char **argv) {

  auto args = std::vector<std::string>(argv + 1, argv + argc);

  compiler::options_t opts;

  std::string file;
  for (size_t i = 0; i < args.size(); i++) {
    if (args[i] == "-p" || args[i] == "--parse-only") {
      opts.parse_only = true;
      continue;
    }

    // If it isn't an option it is a file

    if (file.empty()) {
      file = args[i];
    }
  }

  auto bin =
      compiler::compile_project(compiler::targets_e::SKIFF, {}, file, opts);
  return 0;
}