#include "compiler.hpp"
#include "lang/tokens.hpp"

#include <filesystem>
#include <iostream>

namespace compiler {




std::tuple<bool, results_t>
compile_project(const targets_e target, const options_t options,
                const project::astro_project_t &project) {

  std::cout << "Compile things here" << std::endl;

  std::vector<lang::td_pair_t> tokens;
  
  if (!std::filesystem::is_regular_file(project.bin.entry)) {
    std::cerr << "Configured bin entry '" << project.bin.entry << "' does not exist" << std::endl;
    return {false, {}};
  }

  return {false, {}};
}

} // namespace compiler