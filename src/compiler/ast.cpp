#include "ast.hpp"

#include <iostream>

namespace compiler {

void display_expr_tree(const std::string &prefix, node_c *n, bool is_left) {
  if (!n) {
    return;
  }

  if (n->type == node_type_e::FN) {
    std::cout << "function: " << n->data << std::endl;
    auto fn = reinterpret_cast<function_node_c *>(n);

    if (!fn->params.empty()) {
      std::cout << "params" << std::endl;
    }
    for (auto fnn : fn->params) {
      display_expr_tree(" ", fnn);
    }

    if (!fn->body.empty()) {
      std::cout << "body" << std::endl;
    }
    for (auto fnn : fn->body) {
      display_expr_tree(" ", fnn);
    }
    std::cout << std::endl;
    return;
  }

  if (n->type == node_type_e::FOR) {
    std::cout << " " << n->data << std::endl;
    auto for_loop = reinterpret_cast<for_loop_c *>(n);
    display_expr_tree(" ", for_loop->from);
    display_expr_tree(" ", for_loop->to);
    display_expr_tree(" ", for_loop->step);
    std::cout << " ├── body " << std::endl;
    for (auto body_node : for_loop->body) {
      display_expr_tree("    ", body_node);
    }
    return;
  }

  if (n->type == node_type_e::CONDITIONAL || n->type == node_type_e::ASM) {
    std::cout << prefix;
    std::cout << (is_left ? "├──" : "└──");
    std::cout << " " << n->data << std::endl;
    display_expr_tree(prefix + (is_left ? "│   " : "    "), n->left, true);
    auto print = reinterpret_cast<bodied_node_c *>(n);
    std::cout << " ├── body " << std::endl;
    for (auto body_node : print->body) {
      display_expr_tree("    ", body_node);
    }
    display_expr_tree(prefix + (is_left ? "│   " : "    "), n->right, false);
    return;
  }

  if (n->type == node_type_e::VARIABLE) {
    auto v = reinterpret_cast<variable_c *>(n);
    std::string type = v->type_name.empty() ? "unknown" : v->type_name;
    std::cout << " " << n->data << " (type:" << type << ")" << std::endl;
    std::cout << prefix;
    std::cout << (is_left ? "├──" : "└──");
    std::cout << " " << n->data << std::endl;
    display_expr_tree(prefix + (is_left ? "│   " : "    "), n->left, true);
    display_expr_tree(prefix + (is_left ? "│   " : "    "), n->right, false);
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

  if (node->type == node_type_e::CONDITIONAL ||
      node->type == node_type_e::ASM) {
    auto bn = reinterpret_cast<bodied_node_c *>(node);
    for (auto body_node : bn->body) {
      free_nodes(body_node);
    }
  }

  if (node->type == node_type_e::FOR) {
    auto for_loop = reinterpret_cast<for_loop_c *>(node);
    for (auto body_node : for_loop->body) {
      free_nodes(body_node);
    }
    free_nodes(for_loop->from);
    free_nodes(for_loop->to);
    free_nodes(for_loop->step);
  }

  if (node->type == node_type_e::FN) {
    auto fn = reinterpret_cast<function_node_c *>(node);
    for (auto n : fn->params) {
      free_nodes(n);
    }
    for (auto n : fn->body) {
      free_nodes(n);
    }
  }

  free_nodes(node->left);
  free_nodes(node->right);
  delete node;
}

} // namespace compiler