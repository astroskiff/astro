#include "ast.hpp"

#include <iostream>

namespace compiler {

void display_expr_tree(const std::string &prefix, node_c *n, bool is_left) {
  if (!n) {
    return;
  }

  if (n->type == node_type::FOR) {
    std::cout << " " << n->data << std::endl;
    auto for_loop = reinterpret_cast<for_loop_c*>(n);
    display_expr_tree(" ", for_loop->from);
    display_expr_tree(" ", for_loop->to);
    display_expr_tree(" ", for_loop->step);
    std::cout << " ├── body " << std::endl;
    for(auto body_node : for_loop->body) {
      display_expr_tree("    ", body_node);
    }
    return;
  }

  if (n->type == node_type::PRINT) {
    std::cout << " " << n->data << std::endl;
    auto print = reinterpret_cast<print_c*>(n);
    std::cout << " ├── body " << std::endl;
    for(auto body_node : print->body) {
      display_expr_tree("    ", body_node);
    }
    return;
  }

  std::cout << prefix;
  std::cout << (is_left ? "├──" : "└──");

  std::cout << " " << n->data << std::endl;
  display_expr_tree(prefix + (is_left ? "│   " : "    "), n->left, true);
  display_expr_tree(prefix + (is_left ? "│   " : "    "), n->right, false);
}

void append_node(node_c *to, node_c *from) {
  if (!to) {
    return;
  }
  if (!to->left) {
    to->left = from;
  } else {
    auto i = to;
    while (i->right) {
      i = i->right;
    }
    i->right = from;
  }
}

void free_nodes(node_c *node) {
  if (!node) {
    return;
  }

  if (node->type == node_type::PRINT) {
    auto print = reinterpret_cast<print_c*>(node);
    for(auto body_node : print->body) {
      free_nodes(body_node);
    }
  }

  if (node->type == node_type::FOR) {
    auto for_loop = reinterpret_cast<for_loop_c*>(node);
    for(auto body_node : for_loop->body) {
      free_nodes(body_node);
    }
    free_nodes(for_loop->from);
    free_nodes(for_loop->to);
    free_nodes(for_loop->step);
  }
  free_nodes(node->left);
  free_nodes(node->right);
  delete node;
}

} // namespace compiler