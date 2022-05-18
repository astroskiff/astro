#ifndef COMPILER_MIDDLE_SEMANTIC_ANALYSIS
#define COMPILER_MIDDLE_SEMANTIC_ANALYSIS

#include "compiler/ast.hpp"
#include "compiler/middle/ir.hpp"
#include "compiler/shared/page.hpp"

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace compiler {
namespace middle {

//! \brief Object that performs semantic analysis
class semantic_analysis_c {
public:
  //! \brief Create the sa object with pages of source
  //! \param pages The loaded pages of source code so errors can
  //!        be accurately reported on error
  semantic_analysis_c(std::unordered_map<std::string, shared::page_c> &pages);

  //! \brief Perform analysis on the instructions
  //! \param instructions The instructions to analyze
  //! \returns tuple with true iff all instructions pass analyzation
  //!          ir instructions will only be populated if true is set
  std::tuple<bool, std::vector<ir::ir_instruction_c *>>
  analyze(const std::vector<node_c *> &instructions);

private:
  std::unordered_map<std::string, shared::page_c> &_pages;
  std::vector<ir::ir_instruction_c *> _resulting_instructions;

  void die(uint64_t error_no, std::string error, bool basic_error = false);

  bool _sa_okay{true};

  void analyze_node(node_c * node);
};

} // namespace middle
} // namespace compiler

#endif