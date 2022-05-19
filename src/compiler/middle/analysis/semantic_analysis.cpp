#include "semantic_analysis.hpp"
#include "compiler/shared/reporter.hpp"

#include <iostream>

namespace compiler {
namespace middle {

semantic_analysis_c::semantic_analysis_c(
    std::unordered_map<std::string, shared::page_c> &pages)
    : _pages(pages) {}

std::tuple<bool, std::vector<ir::ir_instruction_c *>>
semantic_analysis_c::analyze(const std::vector<node_c *> &instructions) {
  // Iterate over all instructions given
  // Have them visit this object
  // Each item visiting will be analyzed with the resulting IR
  //    being generated.

  return {false, {}};
}

void semantic_analysis_c::die(node_c *node, uint64_t error_no, std::string error, bool basic_error)
{
  _sa_okay = false;
  compiler::shared::reporter_c reporter(_pages);
  if (!node || basic_error) {
    reporter.standard_report(compiler::shared::base_report_c(
        compiler::shared::report_origin_e::PARSER,
        compiler::shared::level_e::ERROR, error));
  } else {
    reporter.marked_report(compiler::shared::marked_source_report_c(
        compiler::shared::report_origin_e::PARSER,
        compiler::shared::level_e::ERROR, error,
        node->location, error_no));
  }
}

void semantic_analysis_c::analyze_node(node_c *node)
{
  // Within the SA here we should only hit "top level" nodes
  // So.. full statement constructs not operations

  if(!node) {
    return;
  }

  switch(node->type) {
    case node_type_e::FN: { break; }
    case node_type_e::CALL: { break; }
    case node_type_e::ASM: { break; }
    case node_type_e::LET: { break; }
    case node_type_e::REASSIGN: { break; }
    case node_type_e::LABEL: { break; }
    case node_type_e::FOR: { break; }
    case node_type_e::GOTO: { break; }
    case node_type_e::GOSUB: { break; }
    case node_type_e::RETURN: { break; }
    default: {

      //  TODO : Update this to use die() once file / node information can be
      //         tied together
      //
      std::cerr << "Internal Error: SA hit non-statement node" << std::endl;
      _sa_okay = false;
      return;
    }
  }
}

} // namespace middle
} // namespace compiler