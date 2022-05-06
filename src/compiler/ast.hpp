#ifndef COMPILER_AST_HPP
#define COMPILER_AST_HPP

#include <string>

namespace compiler {

// Forward to externed function can be at top for visibility
class node_c;

extern void display_expr_tree(const std::string &prefix, node_c *n,
                              bool is_left = true);

extern void append_node(node_c *to, node_c *from);

extern void free_nodes(node_c *node);

enum class node_type
{ 
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
  NOT
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
  node_c(const node_type type, const location_c loc) : type(type), location(loc){}
  node_c(const node_type type, const location_c loc, const std::string &data) : type(type), data(data), location(loc){}
  node_c *left{nullptr};
  node_c *right{nullptr};
  std::string data;
  node_type type;
  location_c location;
};



} // namespace compiler

#endif