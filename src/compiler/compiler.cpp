#include "compiler.hpp"
#include "front/parser.hpp"
#include <filesystem>
#include <iostream>

#include "shared/expression_eval.hpp"
#include "shared/types.hpp"

namespace compiler {

std::vector<uint8_t>
compile_project(const targets_e target,
                std::vector<std::string> include_directories,
                const std::string &file, const options_t &opts) {

  if (!std::filesystem::is_regular_file(file)) {
    std::cerr << "File: " << file << " does not exist" << std::endl;
    return {};
  }

  std::vector<front::td_pair_t> tokens;

  //  Create the parser with cdt.
  //
  front::parser_c parser;
  std::vector<node_c *> instruction = parser.parse_file(file);


  //  If the parser is dead we need to die
  //   - If the option for parse only is set we 
  //     exit success iff the parser is okay
  if(!parser.is_okay()) {
    exit(EXIT_FAILURE);
  } else if (opts.parse_only) {
    std::cout << "Got " << instruction.size() << " items\n";
    for (auto i : instruction) {
      display_expr_tree("", i);
      if (i->type == compiler::node_type_e::REASSIGN) {
        auto results = shared::evaluate_expression(i->right);
        for (auto r : results.execution_order) {
          std::cout << r->data << " ";
        }
        std::cout << std::endl;
      }
    }
    exit(EXIT_SUCCESS);
  }



  return {};
}

} // namespace compiler