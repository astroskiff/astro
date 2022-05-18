#include "compiler.hpp"
#include "front/parser.hpp"
#include "middle/analysis/semantic_analysis.hpp"
#include "middle/ir.hpp"
#include "shared/page.hpp"
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
  //
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

  //  Call the semantic analyzer to check the code and 
  //  to generate the IR instructions that we will
  //  use to optimize and generate target code
  //
  auto [sa_pass, ir_instructions] = sa.analyze(instructions);
  if (!sa_pass) {
    for (auto instruction : instructions) {
      free_nodes(instruction);
    }
    std::cerr << "Semantic analysis failed" << std::endl;
    return {};
  }



  return {};
}

} // namespace compiler