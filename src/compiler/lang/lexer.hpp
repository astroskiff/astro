#ifndef COMPILER_LANG_LEXER_HPP
#define COMPILER_LANG_LEXER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "tokens.hpp"

namespace compiler {
namespace lang {

//! \brief Lexer
class lexer_c {
public:
  lexer_c();

  //! \brief Clear the lexer
  void clear();

  //! \brief Lex a string and get a series of token/data pairs
  //! \param line_no The line number of the sting
  //! \param line The line to tokenize
  std::vector<td_pair_t> lex(size_t line_no, std::string line);

private:
  std::vector<td_pair_t> _tokens;
  struct SourceLine {
    std::string data;
    size_t line_no;
  };
  std::string _current_line;
  size_t _idx;
  std::unordered_map<std::string, token_e> _reserved;

  void advance();
  char peek(size_t ahead = 1);
};
} // namespace lang
} // namespace compiler

#endif