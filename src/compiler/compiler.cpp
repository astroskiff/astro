#include "compiler.hpp"
#include "lang/common.hpp"
#include "lang/parser.hpp"

#include <iostream>

namespace compiler {

std::tuple<bool, results_t>
compile_project(const targets_e target, const options_t options,
                const project::astro_project_t &project) {

  if (!std::filesystem::is_regular_file(project.bin.entry)) {
    std::cerr << "Configured bin entry '" << project.bin.entry
              << "' does not exist" << std::endl;
    return {false, {}};
  }

  std::cout << "Compile things here" << std::endl;

  std::vector<lang::td_pair_t> tokens;

  lang::compiler_data_t cdt;
  cdt.known_modules = project.bin.modules;

  //  Get the directory that the entry file resides in
  //
  {
    std::filesystem::path main_source_directory = project.bin.entry;
    main_source_directory = main_source_directory.parent_path();
    cdt.operating_directory.push(main_source_directory);
  }

  //  Create the parser with cdt. 
  //  
  lang::parser_c parser(cdt);
  std::vector<lang::instruction_ptr> instruction = parser.parse_file(project.bin.entry);

  return {false, {}};
}

} // namespace compiler