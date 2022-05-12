#include "expression_eval.hpp"

#include "compiler/ast.hpp"

#include <iostream>

namespace compiler {
namespace middle {
namespace analysis {

void walk_expression(compiler::node_c *node,
                     std::vector<node_c *> &execution_order,
                     compiler::shared::base_type_e &type,
                     std::vector<eval_error_t> &errors) {
  if (!node) {
    return;
  }

  switch (node->type) {

  //  Integer Node
  //
  case compiler::node_type_e::INTEGER: {
    switch (type) {
    case compiler::shared::base_type_e::OBJECT: {
      errors.push_back({"User objects are not yet supported", &node->location});
      return;
    }
    case compiler::shared::base_type_e::INT:
      type = compiler::shared::base_type_e::INT;
      break;
    case compiler::shared::base_type_e::BOOL:
      type = compiler::shared::base_type_e::INT;
      break;
    case compiler::shared::base_type_e::DOUBLE:
      type = compiler::shared::base_type_e::DOUBLE;
      break;
    case compiler::shared::base_type_e::STRING:
      type = compiler::shared::base_type_e::STRING;
      break;
    }
    execution_order.push_back(node);
    break;
  }

  //  Float Node
  //
  case compiler::node_type_e::FLOAT: {
    switch (type) {
    case compiler::shared::base_type_e::OBJECT: {
      errors.push_back({"User objects are not yet supported", &node->location});
      return;
    }
    case compiler::shared::base_type_e::INT:
    case compiler::shared::base_type_e::BOOL:
    case compiler::shared::base_type_e::DOUBLE:
      type = compiler::shared::base_type_e::DOUBLE;
      break;
    case compiler::shared::base_type_e::STRING:
      type = compiler::shared::base_type_e::STRING;
      break;
    }
    execution_order.push_back(node);
    break;
  }

  //  String Node
  //
  case compiler::node_type_e::STRING: {
    if (compiler::shared::base_type_e::OBJECT == type) {
      errors.push_back({"User objects are not yet supported", &node->location});
      return;
    }

    type = compiler::shared::base_type_e::STRING; // Everything gets promoted to
                                                  // string
    execution_order.push_back(node);
    break;
  }

  //  Variable Node
  //
  case compiler::node_type_e::VARIABLE: {
    // Check if variable exists ( need the symbol table )
    // Check the variable type to see if things will jive
    execution_order.push_back(node);
    break;
  }

  //  Binary infix operations
  //
  case compiler::node_type_e::ADD: {
    walk_expression(node->left, execution_order, type, errors);
    walk_expression(node->right, execution_order, type, errors);
    execution_order.push_back(node);
    break;
  }
  case compiler::node_type_e::SUB:
  case compiler::node_type_e::DIV:
  case compiler::node_type_e::MUL:
  case compiler::node_type_e::HAT:
  case compiler::node_type_e::OR:
  case compiler::node_type_e::AND:
  case compiler::node_type_e::NOT:
  case compiler::node_type_e::EQ_EQ:
  case compiler::node_type_e::LT:
  case compiler::node_type_e::LTE:
  case compiler::node_type_e::GT:
  case compiler::node_type_e::GTE:
  case compiler::node_type_e::POW:
  case compiler::node_type_e::LSH:
  case compiler::node_type_e::RSH:
  case compiler::node_type_e::MOD:
  case compiler::node_type_e::BW_AND:
  case compiler::node_type_e::BW_OR:
  case compiler::node_type_e::BW_XOR: {
    if (type == compiler::shared::base_type_e::STRING) {
      errors.push_back({"Invalid operation on string type", &node->location});
    }
    walk_expression(node->left, execution_order, type, errors);
    walk_expression(node->right, execution_order, type, errors);
    execution_order.push_back(node);
    break;
  }

  //  Unary prefix operations
  //
  case compiler::node_type_e::BW_NOT:
  case compiler::node_type_e::NOT_EQ: {
    walk_expression(node->left, execution_order, type, errors);
    execution_order.push_back(node);
    break;
  }
  default:
    errors.push_back(
        {"Invalid node type detected in expression", &node->location});
    break;
  }
}

eval_results_t evaluate_expression(compiler::node_c *expression) {
  eval_results_t results;

  walk_expression(expression, results.execution_order, results.resulting_type,
                  results.errors);

  switch (results.resulting_type) {
  case compiler::shared::base_type_e::BOOL:
    results.resulting_type_name = "bool";
    break;
  case compiler::shared::base_type_e::INT:
    results.resulting_type_name = "int";
    break;
  case compiler::shared::base_type_e::DOUBLE:
    results.resulting_type_name = "double";
    break;
  case compiler::shared::base_type_e::STRING:
    results.resulting_type_name = "string";
    break;
  case compiler::shared::base_type_e::OBJECT:
    // Will need to have a storage of user defined objects in the symbol table
    // that can point us to the actual name of the thing
    results.errors.push_back(
        {"User objects are not yet supported", &expression->location});
    results.resulting_type_name = "error";
    break;
  }

  return results;
}

} // namespace analysis

} // namespace middle

} // namespace compiler