#include "semantic_analysis.hpp"

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

void semantic_analysis_c::die(uint64_t error_no, std::string error, bool basic_error)
{
  _sa_okay = false;
  // TODO: 
  //  Need to tie nodes to pages somehow. Perhaps we can embed that into location_c,
  //  but by the time that the nodes get to the SA we've lost which file they are 
  //  created from. 
  //
  //  Pipe to error reporter
  //
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