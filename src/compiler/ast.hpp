#ifndef COMPILER_AST_HPP
#define COMPILER_AST_HPP

#include <string>
#include <vector>

namespace compiler {

// Forward to externed function can be at top for visibility
class node_c;

extern void display_expr_tree(const std::string &prefix, node_c *n,
                              bool is_left = true);

extern void append_node(node_c *to, node_c *from);

extern void free_nodes(node_c *node);

enum class node_type {
  LET,
  ID,
  ADD,
  SUB,
  DIV,
  MUL,
  HAT,
  INTEGER,
  FLOAT,
  OR,
  AND,
  NOT,
  EQ,
  EQ_EQ,
  LT,
  LTE,
  GT,
  GTE,
  POW,
  STRING,
  LABEL,
  FOR,
  END,
  GOTO,
  PRINT
};

class location_c {
public:
  location_c() : line(0), col(0) {}
  location_c(std::size_t line, std::size_t col) : line(line), col(col) {}
  std::size_t line{0};
  std::size_t col{0};
};

class node_c {
public:
  node_c(const node_type type, const location_c loc)
      : type(type), location(loc) {}
  node_c(const node_type type, const location_c loc, const std::string &data)
      : type(type), data(data), location(loc) {}
  node_c *left{nullptr};
  node_c *right{nullptr};
  std::string data;
  node_type type;
  location_c location;
};

class for_loop_c : public node_c {
public:
  for_loop_c(const location_c &loc) : node_c(node_type::FOR, loc) {}

  node_c * from {nullptr};
  node_c * to {nullptr};
  node_c * step {nullptr};
  std::vector<node_c*> body;
};

class print_c : public node_c {
public:
  print_c(const location_c &loc) : node_c(node_type::PRINT, loc) {}
  std::vector<node_c*> body;
};

} // namespace compiler

#endif