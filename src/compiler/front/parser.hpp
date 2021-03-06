#ifndef COMPILER_PARSER_HPP
#define COMPILER_PARSER_HPP

#include "compiler/ast.hpp"
#include "compiler/shared/page.hpp"
#include "tokens.hpp"
#include <vector>

#include <functional>
#include <unordered_map>

namespace compiler {
namespace front {

class parser_c {
public:
  enum class precedence_e {
    LOWEST,
    ASSIGN,       // =
    LOGICAL,      // || &&
    BITWISE,      // or, xor, and, not
    EQUALS,       // ==
    LESS_GREATER, // > <
    SHIFT,        // lsh, rsh
    SUM,          // +
    PROD,         // *
    POW,          // ^
    PREFIX,       // -a or !a
    CALL,         // call()
    INDEX         // []
  };

  parser_c(std::unordered_map<std::string, shared::page_c> &pages);
  std::vector<node_c *> parse_file(const std::string &file);

  bool is_okay() const { return _parser_okay; }
  void mark_parsing_module() { _parsing_module = true; }

private:
  typedef node_c *(parser_c::*prefix_parse_fn)();
  typedef node_c *(parser_c::*infix_parse_fn)(node_c *);

  std::size_t _idx{0};
  std::size_t _mark{0};
  bool _parser_okay{true};
  bool _parsing_module{false};
  std::vector<td_pair_t> _tokens;
  std::string _source_name;
  std::unordered_map<std::string, shared::page_c> &_pages;

  void prev();
  void advance();
  void mark();
  void reset();
  void unset();
  const td_pair_t &current_td_pair() const;
  const td_pair_t &peek(const std::size_t ahead = 1) const;
  void die(uint64_t error_no, std::string error, bool basic_error = false);

  std::vector<std::function<node_c *()>> _statement_functions;

  node_c *function();
  node_c *call_statement();
  node_c *let_statement();
  node_c *label_statement();
  node_c *for_statement();
  node_c *goto_statement();
  node_c *gosub_statement();
  node_c *if_statement();
  node_c *elif_statement();
  node_c *else_statement();
  node_c *return_statement();
  node_c *asm_statement();
  node_c *reassign_statement();

  std::vector<node_c *> function_parameters();
  std::vector<node_c *> statement_block();
  std::vector<node_c *> call_parameters();

  precedence_e peek_precedence();
  std::unordered_map<token_e, prefix_parse_fn> _prefix_fns;
  std::unordered_map<token_e, infix_parse_fn> _infix_fns;
  node_c *expression(precedence_e precedence);
  node_c *prefix_expr();
  node_c *grouped_expr();
  node_c *constructor_list();
  node_c *infix_expr(node_c *left);
  node_c *identifier();
  node_c *call();
  node_c *array_index();
  node_c *number();
  node_c *str();
};

} // namespace front
} // namespace compiler

#endif