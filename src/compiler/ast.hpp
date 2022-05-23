#ifndef COMPILER_AST_HPP
#define COMPILER_AST_HPP

#include <string>
#include <vector>

#include "shared/common.hpp"

namespace compiler {

// Forward to externed function can be at top for visibility
class node_c;

extern void display_expr_tree(const std::string &prefix, node_c *n,
                              bool is_left = true);

extern void append_node(node_c *to, node_c *from);

extern void free_nodes(node_c *node);

enum class data_type_e { UNKNOWN = 0, INT, FLOAT, STRING, USER_DECL };

extern data_type_e string_to_data_type(const std::string &type_name);

extern data_type_e data_type_from_string(const std::string &data);

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
  GOSUB,
  RETURN,
  NOT_EQ,
  CONDITIONAL
};

class node_c {
public:
  node_c(const node_type_e type, const shared::location_c loc)
      : type(type), location(loc) {}
  node_c(const node_type_e type, const shared::location_c loc,
         const std::string &data)
      : type(type), data(data), location(loc) {}
  node_c *left{nullptr};
  node_c *right{nullptr};
  std::string data;
  node_type_e type;
  shared::location_c location;
};

class variable_c : public node_c {
public:
  variable_c(const shared::location_c &loc, const std::string data)
      : node_c(node_type_e::VARIABLE, loc, data) {}
  variable_c(const shared::location_c &loc, const std::string data,
             const std::string type)
      : node_c(node_type_e::VARIABLE, loc, data), type_name(type), data_type(data_type_from_string(type)) {}

  std::string type_name;
  data_type_e data_type;
};

class for_loop_c : public node_c {
public:
  for_loop_c(const shared::location_c &loc) : node_c(node_type_e::FOR, loc) {}

  node_c *from{nullptr};
  node_c *to{nullptr};
  node_c *step{nullptr};
  std::vector<node_c *> body;
};

class bodied_node_c : public node_c {
public:
  bodied_node_c(const node_type_e type, const shared::location_c &loc)
      : node_c(type, loc) {}
  bodied_node_c(const node_type_e type, const shared::location_c &loc,
                const std::string name, const std::vector<node_c *> &body)
      : node_c(type, loc, name), body(body) {}
  bodied_node_c(const node_type_e type, const shared::location_c &loc,
                const std::vector<node_c *> &body)
      : node_c(type, loc), body(body) {}
  std::vector<node_c *> body;
};

class function_node_c : public node_c {
public:
  function_node_c(const shared::location_c &loc, const std::string name,
                  const std::vector<node_c *> &body,
                  const std::vector<node_c *> &params,
                  const std::string return_type_name,
                  const shared::location_c &return_type_loc)
      : node_c(node_type_e::FN, loc, name), body(body), params(params),
        return_type(return_type_name), data_type(data_type_from_string(return_type)), return_type_loc(return_type_loc) {}

  std::string return_type;
  data_type_e data_type;
  shared::location_c return_type_loc;
  std::vector<node_c *> params;
  std::vector<node_c *> body;
};

} // namespace compiler

#endif