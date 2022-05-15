#include "expression_eval.hpp"

#include "compiler/ast.hpp"

#include <iostream>

namespace compiler {
namespace middle {
namespace analysis {

expression_evaluator_c::expression_evaluator_c(scoped_symbol_table_c &table)
    : _symbol_table(table) {}

expression_evaluator_c::eval_results_t expression_evaluator_c::evaluate(
    node_c *expression, std::optional<compiler::data_type_e> declared_as) {
  _execution_order.clear();
  _errors.clear();
  _resulting_type = compiler::data_type_e::UNKNOWN;

  eval_results_t results;
  if (!expression) {
    results.errors.push_back(
        {"Internal: Invalid pointer to expression", nullptr});
    return results;
  }

  return results;
}

void expression_evaluator_c::walk_expression(node_c *node) {
  if (!node) {
    return;
  }

  switch (node->type) {

  //  Data type nodes
  //
  case compiler::node_type_e::INTEGER:
  case compiler::node_type_e::FLOAT:
  case compiler::node_type_e::STRING:
  case compiler::node_type_e::VARIABLE: {
    check_promotion(node);
    _execution_order.push_back(node);
    break;
  }

    // Function call
    //

  case compiler::node_type_e::CALL: {
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
    if (_resulting_type == compiler::data_type_e::STRING) {
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

void expression_evaluator_c::check_promotion(compiler::node_c *node) {
  switch (node->type) {

  //  raw integer
  //
  case node_type_e::INTEGER: {
    switch (_resulting_type) {
    case compiler::data_type_e::INT: {
      break;
    }
    case compiler::data_type_e::UNKNOWN: {
      _resulting_type = compiler::data_type_e::INT;
      break;
    }
    case compiler::data_type_e::FLOAT: {
      break;
    }
    case compiler::data_type_e::STRING: {
      _errors.push_back(
          {"Invalid type given when operating with a string", &node->location});
      break;
    }
    case compiler::data_type_e::USER_DECL: {
      _errors.push_back(
          {"User objects are not yet supported", &node->location});
      break;
    }
    }
    break;
  }
  case node_type_e::FLOAT: {
    switch (_resulting_type) {
    case compiler::data_type_e::INT:
    case compiler::data_type_e::UNKNOWN: {
      _resulting_type = compiler::data_type_e::FLOAT;
      break;
    }
    case compiler::data_type_e::FLOAT: {
      break;
    }
    case compiler::data_type_e::STRING: {
      _errors.push_back(
          {"Invalid type given when operating with a string", &node->location});
      break;
    }
    case compiler::data_type_e::USER_DECL: {
      _errors.push_back(
          {"User objects are not yet supported", &node->location});
      break;
    }
    }
    break;
  }
  case node_type_e::STRING: {
    _resulting_type = compiler::data_type_e::STRING;
  }

  case node_type_e::CALL: {

    auto cs = _symbol_table.get_current_scope();
    if (!cs) {
      _errors.push_back(
          {"Internal Error: No scope present during expression evaluation",
           &node->location});
      break;
    }

    auto data = cs->find(node->data);
    if (!data) {
      _errors.push_back({"Unknown function: " + node->data, &node->location});
      break;
    }

    if (data->type != entry_c::entry_type_e::FUNCTION) {
      _errors.push_back({"Non-function name given to expression: " + node->data,
                         &node->location});
      break;
    }

    auto fn = reinterpret_cast<function_c *>(data);

    switch (fn->return_type) {
    case compiler::data_type_e::INT: {
      auto n = new node_c(node_type_e::INTEGER, node->location);
      check_promotion(n);
      delete n;
      break;
    }
    case compiler::data_type_e::FLOAT: {
      auto n = new node_c(node_type_e::FLOAT, node->location);
      check_promotion(n);
      delete n;
      break;
    }
    case compiler::data_type_e::STRING: {
      auto n = new node_c(node_type_e::STRING, node->location);
      check_promotion(n);
      delete n;
      break;
    }
    case compiler::data_type_e::UNKNOWN: {
      _errors.push_back(
          {"Return type of given function is unknown at evaluation time",
           &node->location});
      break;
      break;
    }
    case compiler::data_type_e::USER_DECL: {
      _errors.push_back(
          {"User objects are not yet supported", &node->location});
      break;
    }
    }
    break;
  }

  case node_type_e::VARIABLE: {
    // Load the variable and returnt a call into this function of the type

    auto cs = _symbol_table.get_current_scope();
    if (!cs) {
      _errors.push_back(
          {"Internal Error: No scope present during expression evaluation",
           &node->location});
      break;
    }

    auto data = cs->find(node->data);
    if (!data) {
      _errors.push_back({"Unknown variable: " + node->data, &node->location});
      break;
    }

    if (data->type != entry_c::entry_type_e::MEMBER) {
      _errors.push_back(
          {"Non-variable member given to expression: " + node->data,
           &node->location});
      break;
    }

    auto member = reinterpret_cast<type_c *>(data);

    switch (member->type) {
    case compiler::data_type_e::INT: {
      auto n = new node_c(node_type_e::INTEGER, node->location);
      check_promotion(n);
      delete n;
      break;
    }
    case compiler::data_type_e::FLOAT: {
      auto n = new node_c(node_type_e::FLOAT, node->location);
      check_promotion(n);
      delete n;
      break;
    }
    case compiler::data_type_e::STRING: {
      auto n = new node_c(node_type_e::STRING, node->location);
      check_promotion(n);
      delete n;
      break;
    }
    case compiler::data_type_e::UNKNOWN: {
      _errors.push_back({"Type of given variable is unknown at evaluation time",
                         &node->location});
      break;
      break;
    }
    case compiler::data_type_e::USER_DECL: {
      _errors.push_back(
          {"User objects are not yet supported", &node->location});
      break;
    }
    }
    // Outer switch break
    break;
  }
  default: {
    std::cerr << "Internal error : Non-var type AST node given to promotion "
                 "checker\n";
    exit(EXIT_FAILURE);
  }
  }
}

} // namespace analysis

} // namespace middle

} // namespace compiler