#ifndef COMPILER_PARSER_HPP
#define COMPILER_PARSER_HPP

#include "ast.hpp"
#include "tokens.hpp"
#include <vector>

#include <functional>
#include <unordered_map>

namespace compiler {

class parser_c {
public:
  enum class precedence_e {
    LOWEST,
    ASSIGN,       // =
    LOGICAL,      // || &&
    EQUALS,       // ==
    LESS_GREATER, // > <
    SUM,          // +
    PROD,         // *
    POW,          // ^
    PREFIX,       // -a or !a
  };

  parser_c();
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

  void prev();
  void advance();
  void mark();
  void reset();
  void unset();
  void expect(const token_e token, const std::string &error,
              const size_t ahead = 0);
  const td_pair_t &current_td_pair() const;
  const td_pair_t &peek(const std::size_t ahead = 1) const;
  void die(uint64_t error_no, std::string error);

  std::vector<std::function<node_c *()>> _statement_functions;

  node_c *statement();
  node_c *let_statement();
  node_c *label_statement();
  node_c *for_statement();
  node_c *goto_statement();
  node_c *gosub_statement();
  node_c *if_statement();
  node_c *elif_statement();
  node_c *else_statement();
  node_c *print_statement();
  node_c *read_statement();
  node_c *return_statement();

  std::vector<node_c *> statement_block();

  precedence_e peek_precedence();
  std::unordered_map<token_e, prefix_parse_fn> _prefix_fns;
  std::unordered_map<token_e, infix_parse_fn> _infix_fns;
  node_c *expression(precedence_e precedence);
  node_c *prefix_expr();
  node_c *grouped_expr();
  node_c *infix_expr(node_c *left);
  node_c *identifier();
  node_c *number();
  node_c *str();
};

} // namespace compiler

#endif