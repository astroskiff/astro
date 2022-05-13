#ifndef COMPILER_EXP_EVAL_HPP
#define COMPILER_EXP_EVAL_HPP

#include "compiler/shared/common.hpp"
#include "scoped_symbol_table.hpp"
#include "types.hpp"
#include <string>
#include <vector>
#include <optional>

namespace compiler {

// FWD Declaration for things in compiler
class node_c;
class location_c;

namespace middle {
namespace analysis {

//! \brief Expression evaluation class
class expression_evaluator_c {
public:
  //! \brief Errors that may arrise during evauluation
  struct eval_error_t {
    std::string error;
    compiler::shared::location_c *location;
  };

  //! \brief Reults of evaluation
  struct eval_results_t {
    bool is_valid{false};
    base_forward_types_e resulting_type;             // Base type enum representation
    std::string resulting_type_name;                 // In case its user defined
    std::vector<compiler::node_c *> execution_order; // Pointers into tree data
    std::vector<eval_error_t> errors;
  };

  expression_evaluator_c(scoped_symbol_table_c &symbol_table);

  //! \brief Evaluate a given expression
  //! \param expression The expression to evaluate
  //! \param declared_as The declared type (iff) the type was actually declared
  //! \returns eval_results_t containing all information extracted
  //!          from the given expression
  eval_results_t evaluate(node_c * expression, std::optional<base_forward_types_e> declared_as);

private:
  scoped_symbol_table_c &_symbol_table;
  std::vector<node_c *> _execution_order;
  std::vector<expression_evaluator_c::eval_error_t> _errors;
  base_forward_types_e _resulting_type;


  void walk_expression(compiler::node_c *node);
};

} // namespace analysis
} // namespace middle
} // namespace compiler

#endif