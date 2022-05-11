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

enum class node_type_e {
  FN,
  CALL,
  ASM,
  LET,
  REASSIGN,
  ID,
  VARIABLE,
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
  LSH,
  RSH,
  MOD,
  BW_AND,
  BW_OR,
  BW_NOT,
  BW_XOR,
  STRING,
  LABEL,
  FOR,
  GOTO,
  RETURN,
  NOT_EQ,
  CONDITIONAL
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
  node_c(const node_type_e type, const location_c loc)
      : type(type), location(loc) {}
  node_c(const node_type_e type, const location_c loc, const std::string &data)
      : type(type), data(data), location(loc) {}
  node_c *left{nullptr};
  node_c *right{nullptr};
  std::string data;
  node_type_e type;
  location_c location;
};

class variable_c : public node_c {
public:
  variable_c(const location_c &loc, const std::string data)
      : node_c(node_type_e::VARIABLE, loc, data) {}
  variable_c(const location_c &loc, const std::string data,
             const std::string type)
      : node_c(node_type_e::VARIABLE, loc, data), type_name(type) {}

  std::string type_name;
};

class for_loop_c : public node_c {
public:
  for_loop_c(const location_c &loc) : node_c(node_type_e::FOR, loc) {}

  node_c *from{nullptr};
  node_c *to{nullptr};
  node_c *step{nullptr};
  std::vector<node_c *> body;
};

class bodied_node_c : public node_c {
public:
  bodied_node_c(const node_type_e type, const location_c &loc)
      : node_c(type, loc) {}
  bodied_node_c(const node_type_e type, const location_c &loc,
                const std::string name, const std::vector<node_c *> &body)
      : node_c(type, loc, name), body(body) {}
  bodied_node_c(const node_type_e type, const location_c &loc,
                const std::vector<node_c *> &body)
      : node_c(type, loc), body(body) {}
  std::vector<node_c *> body;
};

class function_node_c : public node_c {
public:
  function_node_c(const location_c &loc, const std::string name,
                  const std::vector<node_c *> &body,
                  const std::vector<node_c *> &params,
                  const std::string return_type_name,
                  const location_c &return_type_loc)
      : node_c(node_type_e::FN, loc, name), body(body), params(params),
        return_type(return_type_name), return_type_loc(return_type_loc) {}

  std::string return_type;
  location_c return_type_loc;
  std::vector<node_c *> params;
  std::vector<node_c *> body;
};

} // namespace compiler

#endif