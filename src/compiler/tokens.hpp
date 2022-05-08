#ifndef COMPILER_TOKENS_HPP
#define COMPILER_TOKENS_HPP

#include "ast.hpp"
#include <string>

namespace compiler {

enum class token_e {
  ID,
  FOR,
  TO,
  STEP,
  GOTO,
  GOSUB,
  IF,
  THEN,
  ELIF,
  ELSE,
  LET,
  PRINT,
  OCTOTHORP,
  READ,
  RETURN,
  NOT,
  AND,
  OR,
  COMMA,
  EQ,
  EQ_EQ,
  NOT_EQ,
  GT,
  GTE,
  LT,
  LTE,
  ADD,
  SUB,
  DIV,
  MUL,
  HAT,
  L_PAREN,
  R_PAREN,
  INTEGER,
  FLOAT,
  STRING,
  LABEL,
  SEMICOLON,
  L_BRACE,
  R_BRACE,
  ERT, // Error token
  EOS  // End of stream
};

//! \brief Token/data pair
struct td_pair_t {
  token_e token;
  std::string data;
  location_c location;
};

//! \brief Print a token data pair
extern std::string token_to_str(const td_pair_t &td);

} // namespace compiler

#endif