#ifndef COMPILER_EXP_EVAL_HPP
#define COMPILER_EXP_EVAL_HPP

#include "compiler/shared/common.hpp"
#include "compiler/shared/types.hpp"
#include <string>
#include <vector>

namespace compiler {

// FWD Declaration for things in compiler
class node_c;
class location_c;

namespace shared {

struct eval_error_t {
  std::string error;
  compiler::shared::location_c *location;
};

struct eval_results_t {
  bool is_valid{false};
  base_type_e resulting_type;      // Base type enum representation
  std::string resulting_type_name; // In case its user defined
  std::vector<compiler::node_c *> execution_order; // Pointers into tree data
  std::vector<eval_error_t> errors;
};

extern eval_results_t evaluate_expression(compiler::node_c *expression);

} // namespace shared
} // namespace compiler

#endif