#include "expression_eval.hpp"

#include "compiler/ast.hpp"

#include <iostream>

namespace compiler {
namespace middle {
namespace analysis {

expression_evaluator_c::expression_evaluator_c(scoped_symbol_table_c &table) : _symbol_table(table)
{

}

expression_evaluator_c::eval_results_t expression_evaluator_c::evaluate(node_c *expression, std::optional<base_forward_types_e> declared_as) 
{
  _execution_order.clear();
  _errors.clear();
  _resulting_type = base_forward_types_e::INT;
  
  eval_results_t results;
  if (!expression) {
    results.errors.push_back({"Internal: Invalid pointer to expression", nullptr});
    return results;
  }

  return results;
}

void expression_evaluator_c::walk_expression(node_c * node)
{
  if (!node) {
    return;
  }

  switch (node->type) {
    

  //  Integer Node
  //
  case compiler::node_type_e::INTEGER: {
    switch (_resulting_type) {
    case base_forward_types_e::USER_DECL: {
      _errors.push_back({"User objects are not yet supported", &node->location});
      return;
    }
    case base_forward_types_e::U8:
    case base_forward_types_e::U16:
    case base_forward_types_e::U32:
    case base_forward_types_e::U64:
    case base_forward_types_e::I8:
    case base_forward_types_e::I16:
    case base_forward_types_e::I32:
    case base_forward_types_e::I64:
    case base_forward_types_e::UINT:
    case base_forward_types_e::INT:
      _resulting_type = base_forward_types_e::INT;
      break;
    case base_forward_types_e::BOOL:
      _resulting_type = base_forward_types_e::INT;
      break;
    case base_forward_types_e::DOUBLE:
      _resulting_type = base_forward_types_e::DOUBLE;
      break;
    case base_forward_types_e::STRING:
      _resulting_type = base_forward_types_e::STRING;
      break;
    }
    _execution_order.push_back(node);
    break;
  }

  //  Float Node
  //
  case compiler::node_type_e::FLOAT: {
    switch (_resulting_type) {
    case base_forward_types_e::USER_DECL: {
      _errors.push_back({"User objects are not yet supported", &node->location});
      return;
    }
    case base_forward_types_e::U8:
    case base_forward_types_e::U16:
    case base_forward_types_e::U32:
    case base_forward_types_e::U64:
    case base_forward_types_e::I8:
    case base_forward_types_e::I16:
    case base_forward_types_e::I32:
    case base_forward_types_e::I64:
    case base_forward_types_e::UINT:
    case base_forward_types_e::INT:
    case base_forward_types_e::BOOL:
    case base_forward_types_e::DOUBLE:
      _resulting_type = base_forward_types_e::DOUBLE;
      break;
    case base_forward_types_e::STRING:
      _resulting_type = base_forward_types_e::STRING;
      break;
    }
    _execution_order.push_back(node);
    break;
  }

  //  String Node
  //
  case compiler::node_type_e::STRING: {
    if (base_forward_types_e::USER_DECL == _resulting_type) {
      _errors.push_back({"User objects are not yet supported", &node->location});
      return;
    }

    _resulting_type = base_forward_types_e::STRING; // Everything gets promoted to
                                                    // string
    _execution_order.push_back(node);
    break;
  }

  //  Variable Node
  //
  case compiler::node_type_e::VARIABLE: {
    // Check if variable exists ( need the symbol table )
    // Check the variable type to see if things will jive
    _execution_order.push_back(node);
    break;
  }

  //  Binary infix operations
  //
  case compiler::node_type_e::ADD: {
    walk_expression(node->left);
    walk_expression(node->right);
    _execution_order.push_back(node);
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
    if (_resulting_type == base_forward_types_e::STRING) {
      _errors.push_back({"Invalid operation on string type", &node->location});
    }
    walk_expression(node->left);
    walk_expression(node->right);
    _execution_order.push_back(node);
    break;
  }

  //  Unary prefix operations
  //
  case compiler::node_type_e::BW_NOT:
  case compiler::node_type_e::NOT_EQ: {
    walk_expression(node->left);
    _execution_order.push_back(node);
    break;
  }
  default:
    _errors.push_back(
        {"Invalid node type detected in expression", &node->location});
    break;
  }
}

} // namespace analysis

} // namespace middle

} // namespace compiler