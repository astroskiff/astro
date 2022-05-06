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

void append_node(node_c *to, node_c *from)
{
  if(!to) { return; }
  if(!to->left) {
    to->left = from;
  } else {
    auto i = to;
    while(i->right) {
      i = i->right;
    }
    i->right = from;
  }
}

void free_nodes(node_c * node) {
  if (!node) {
    return;
  }
  free_nodes(node->left);
  free_nodes(node->right);
  delete node;
}

} // namespace compiler