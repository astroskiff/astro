#include "astro.hpp"

#include <iostream>

namespace compiler {
namespace lang {

void function_c::visit(instruction_receiver_c &ir) { ir.receive(*this); }
void assignment_instruction_c::visit(instruction_receiver_c &ir) {
  ir.receive(*this);
}

void display_expr_tree(const std::string &prefix, expression_c *n,
                       bool is_left) {
  if (!n) {
    return;
  }
  std::cout << prefix;
  std::cout << (is_left ? "├──" : "└──");

  /*
    if (n->type == node_type::CALL) {
      auto i = reinterpret_cast<function_call_expr *>(n);
      if (i->fn) {
        std::cout << " call<" << i->fn->value << ">" << std::endl;
      }
      else {
        std::cout << " call " << std::endl;
      }
    }
    else if (n->type == node_type::ARRAY_IDX) {
      auto i = reinterpret_cast<array_index_expr *>(n);
      if (i->arr && i->index) {
        std::cout << " " << i->arr->value << "[" << i->index->value << "]"
                  << std::endl;
      }
      else {
        std::cout << " array[] " << std::endl;
      }
    }
    else */
  if (n->type == expression_node_type_e::INFIX) {
    auto i = reinterpret_cast<infix_expression_c *>(n);
    std::cout << " " << i->op << std::endl;
    display_expr_tree(prefix + (is_left ? "│   " : "    "), i->left.get(),
                      true);
    display_expr_tree(prefix + (is_left ? "│   " : "    "), i->right.get(),
                      false);
  } else if (n->type == expression_node_type_e::PREFIX) {
    auto i = reinterpret_cast<prefix_expression_c *>(n);
    std::cout << " " << i->op << std::endl;
    display_expr_tree(prefix + (is_left ? "│   " : "    "), i->right.get(),
                      false);
  } else {
    std::cout << " " << n->value << std::endl;
  }
}

} // namespace lang
} // namespace compiler