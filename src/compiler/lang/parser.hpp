#ifndef COMPILER_LANG_PARSER_HPP
#define COMPILER_LANG_PARSER_HPP

#include "common.hpp"
#include <vector>
#include "astro.hpp"
#include "tokens.hpp"

#include <unordered_map>

namespace compiler {
namespace lang {

class parser_c {
public:
  enum class precedence_e {
    LOWEST,
    ASSIGN,       // =
    LOGICAL,      // || && 
    BITWISE,      // ^ | &
    EQUALS,       // ==
    LESS_GREATER, // > <
    SHIFT,        // >> << 
    SUM,          // +
    PROD,         // *
    POW,          // **
    PREFIX,       // -a or !a
    CALL,         // call()
    INDEX         // []
  };

  parser_c(compiler_data_t &cdt);
  std::vector<instruction_ptr> parse_file(const std::string &file);

  bool is_okay() const { return _parser_okay; }
  void mark_parsing_module() { _parsing_module = true; }

private:
  typedef expression_ptr (parser_c::*prefix_parse_fn)();
  typedef expression_ptr (parser_c::*infix_parse_fn)(expression_ptr);

  std::unordered_map<token_e, prefix_parse_fn> _prefix_fns;
  std::unordered_map<token_e, infix_parse_fn> _infix_fns;

  std::size_t _idx{0};
  std::size_t _mark{0};
  bool _parser_okay{true};
  bool _parsing_module{false};
  compiler_data_t &_cdt;
  std::vector<lang::td_pair_t> _tokens;
  std::string _source_name;

  void prev();
  void advance();
  void mark();
  void reset();
  void unset();
  void expect(const token_e token, const std::string &error, const size_t ahead = 0);
  const lang::td_pair_t &current_td_pair() const;
  const lang::td_pair_t &peek(const std::size_t ahead = 1) const;
  precedence_e peek_precedence();
  void die(uint64_t error_no, std::string error);

  std::vector<lang::instruction_ptr> use_statement();
  std::vector<lang::instruction_ptr> import_statement();

  lang::instruction_ptr top_level_item();
  lang::instruction_ptr function();
};

} // namespace lang
} // namespace compiler

#endif