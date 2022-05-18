#include "compiler.hpp"
#include "front/parser.hpp"
#include "shared/page.hpp"
#include "middle/analysis/semantic_analysis.hpp"
#include <filesystem>
#include <iostream>

namespace compiler {

std::vector<uint8_t>
compile_project(const targets_e target,
                std::vector<std::string> include_directories,
                const std::string &file, const options_t &opts) {

  if (!std::filesystem::is_regular_file(file)) {
    std::cerr << "File: " << file << " does not exist" << std::endl;
    return {};
  }

  // Pages of everything that gets loaded from source
  //
  std::unordered_map<std::string, shared::page_c> pages;

  // Parser that will generate instructions from source code
  //
  front::parser_c parser(pages);
  
  //  Generate the source 
  //
  std::vector<node_c *> instructions = parser.parse_file(file);

  //  If the parser is dead we need to die
  //   - If the option for parse only is set we
  //     exit success iff the parser is okay
  if (!parser.is_okay()) {
    exit(EXIT_FAILURE);
  } else if (opts.parse_only) {
    std::cout << "Got " << instructions.size() << " items\n";
    for (auto i : instructions) {
      display_expr_tree("", i);
    }
    exit(EXIT_SUCCESS);
  }

  //  Analyse the trees and ensure everything is valid
  //
  compiler::middle::semantic_analysis_c sa(pages);


  // TODO: Need to update the SA tool to somehow hand back a 
  //       a vector of IR code. Perhaps we can pass in a vector
  //       to fill in, or we can have it return it from the 
  //       analyze function


  if (!sa.analyze(instructions)) {
    for(auto instruction : instructions) {
      free_nodes(instruction);
    }
    std::cerr << "Semantic analysis failed" << std::endl;
    return {};
  }

  // Create the scope thing and type db etc etc

  // Call the analyzer and if everything is fine continue

  return {};
}

} // namespace compiler