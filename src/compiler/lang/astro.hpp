#ifndef COMPILER_LANG_ASTRO_HPP
#define COMPILER_LANG_ASTRO_HPP

#include "tokens.hpp"
#include "types.hpp"
#include <string>

namespace compiler {
namespace lang {

// Forward for item that will reciece instruction objects
// to generate them
class instruction_receiver_c;

// Forward to externed function can be at top for visibility
class expression_c;

/*
 *  prefix / infix and other expression implementations are used to construct
 * expression trees this method will display the expression trees horizontally
 * via stdout
 */
extern void display_expr_tree(const std::string &prefix, expression_c *n,
                              bool is_left);

class variable_c {
public:
  variable_c(const std::string &name, types::base_type_ptr base_type)
      : name(name), base_type(std::move(base_type)) {}
  std::string name;
  types::base_type_ptr base_type;
};
using variable_ptr = std::unique_ptr<variable_c>;

class struct_def_c {
public:
  struct_def_c(const std::string &name,
               std::unordered_map<std::string, types::type_e> &members)
      : name(name), members(members) {}
  std::string name;
  std::unordered_map<std::string, types::type_e> members;

  void add_members(const std::string &name, types::type_e type) {
    members[name] = type;
  }

  bool has_member(const std::string &name) {
    return members.find(name) != members.end();
  }
};

class location_c {
public:
  location_c() : line(0), col(0) {}
  location_c(std::size_t line, std::size_t col) : line(line), col(col) {}
  std::size_t line;
  std::size_t col;
};

//
//    Expressions
//

enum class expression_node_type_e {
  ROOT,
  IDENTIFIER,
  CALL,
  RAW_FLOAT,
  RAW_NUMBER,
  PREFIX,
  INFIX,
};

class expression_c {
public:
  expression_c() : type(expression_node_type_e::ROOT) {}
  expression_c(expression_node_type_e t) : type(t) {}
  expression_c(const location_c &location, expression_node_type_e t)
      : location(location), type(t) {}
  expression_c(expression_node_type_e t, std::string val)
      : type(t), value(val) {}
  expression_c(const location_c &location, expression_node_type_e t,
               std::string val)
      : location(location), type(t), value(val) {}
  virtual ~expression_c() = default;

  location_c location;
  expression_node_type_e type;
  std::string value;
};
using expression_ptr = std::unique_ptr<expression_c>;

class raw_integer_expression_c : public expression_c {
public:
  raw_integer_expression_c(const location_c &location,
                           const std::string &string_value)
      : expression_c(location, expression_node_type_e::RAW_NUMBER,
                     string_value) {}
  raw_integer_expression_c(location_c location, const std::string &string_value,
                           long long raw_value)
      : expression_c(location, expression_node_type_e::RAW_NUMBER,
                     string_value),
        integer_value(raw_value) {}
  long long integer_value{0};
};
using raw_integer_expression_ptr = std::unique_ptr<raw_integer_expression_c>;

class prefix_expression_c : public expression_c {
public:
  prefix_expression_c(const location_c &location, const std::string &op,
                      expression_ptr right)
      : expression_c(location, expression_node_type_e::PREFIX), op(op),
        right(std::move(right)) {}
  std::string op;
  expression_ptr right;
};
using prefix_expr_ptr = std::unique_ptr<prefix_expression_c>;

class infix_expression_c : public expression_c {
public:
  infix_expression_c(const location_c &location, const std::string &op,
                     expression_ptr left, expression_ptr right)
      : expression_c(location, expression_node_type_e::INFIX), op(op),
        left(std::move(left)), right(std::move(right)) {}

  std::string op;
  expression_ptr left;
  expression_ptr right;
};
using infix_expression_ptr = std::unique_ptr<infix_expression_c>;

//
//    Instructions
//

class instruction_c {
public:
  instruction_c() = delete;
  virtual ~instruction_c() = default;
  instruction_c(const location_c &loc) : location(loc) {}
  location_c location;
  virtual void visit(instruction_receiver_c &receiver) = 0;
};
using instruction_ptr = std::unique_ptr<instruction_c>;

class assignment_instruction_c : public instruction_c {
public:
  assignment_instruction_c(const location_c &location, variable_ptr variable,
                           expression_ptr expression)
      : instruction_c(location), variable(std::move(variable)),
        expression(std::move(expression)) {}

  variable_ptr variable;
  expression_ptr expression;

  virtual void visit(instruction_receiver_c &v) override;
};
using assignment_instruction_ptr = std::unique_ptr<assignment_instruction_c>;

class function_c : public instruction_c {
public:
  function_c(const location_c &location) : instruction_c(location) {}
  std::string name;
  std::string file_name;
  variable_ptr return_data;
  std::vector<variable_ptr> parameters;
  std::vector<instruction_ptr> instruction_list;
  virtual void visit(instruction_receiver_c &receiver) override;
};
using function_ptr = std::unique_ptr<function_c>;

//
//    Receiver
//

class instruction_receiver_c {
public:
  virtual void receive(function_c &instruction) = 0;
  virtual void receive(assignment_instruction_c &instruction) = 0;
};

} // namespace lang
} // namespace compiler

#endif