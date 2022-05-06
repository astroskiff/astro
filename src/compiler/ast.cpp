#include "ast.hpp"

#include <iostream>

namespace compiler {

void display_expr_tree(const std::string &prefix, node_c *n,
                       bool is_left) {
  if (!n) {
    return;
  }
  std::cout << prefix;
  std::cout << (is_left ? "├──" : "└──");

  std::cout << " " << n->data << std::endl;
  display_expr_tree(prefix + (is_left ? "│   " : "    "), n->left,
                    true);
  display_expr_tree(prefix + (is_left ? "│   " : "    "), n->right,
                    false);
}

} // namespace compiler