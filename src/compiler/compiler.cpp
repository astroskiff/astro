#include "compiler.hpp"

#include <iostream>

namespace compiler {

std::tuple<bool, results_t>
compile_project(const targets_e target, const options_t options,
                const project::astro_project_t &project) {



  std::cout << "Compile things here" << std::endl;  



  return {false, {}};
}

} // namespace compiler