#include "semantic_analysis.hpp"

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

} // namespace middle
} // namespace compiler