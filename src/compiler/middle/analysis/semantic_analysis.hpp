#ifndef COMPILER_MIDDLE_SEMANTIC_ANALYSIS
#define COMPILER_MIDDLE_SEMANTIC_ANALYSIS

#include "compiler/ast.hpp"
#include "compiler/shared/page.hpp"

#include <string>
#include <unordered_map>

namespace compiler {
namespace middle {

//! \brief Object that performs semantic analysis
class semantic_analysis_c {
public:
  //! \brief Create the sa object with pages of source
  //! \param pages The loaded pages of source code so errors can
  //!        be accurately reported on error
  semantic_analysis_c(std::unordered_map<std::string, shared::page_c>& pages);

  //! \brief Perform analysis on the instructions
  //! \param instructions The instructions to analyze
  //! \returns true iff all instructions pass analyzation
  bool analyze(const std::vector<node_c *> &instructions);

private:
  std::unordered_map<std::string, shared::page_c> &_pages;
};

} // namespace middle
} // namespace compiler

#endif