#ifndef COMPILER_PARSER_HPP
#define COMPILER_PARSER_HPP

#include <vector>
#include "ast.hpp"
#include "tokens.hpp"

#include <unordered_map>

namespace compiler {

class parser_c {
public:
  parser_c();
  std::vector<node_c*> parse_file(const std::string &file);

  bool is_okay() const { return _parser_okay; }
  void mark_parsing_module() { _parsing_module = true; }

private:
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
  void expect(const token_e token, const std::string &error, const size_t ahead = 0);
  const td_pair_t &current_td_pair() const;
  const td_pair_t &peek(const std::size_t ahead = 1) const;
  void die(uint64_t error_no, std::string error);

  node_c* top_level_item();
  node_c* function();
};

} // namespace compiler

#endif