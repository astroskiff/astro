#ifndef COMPILER_TOKENS_HPP
#define COMPILER_TOKENS_HPP

#include "compiler/ast.hpp"
#include <string>

namespace compiler {
namespace front {

enum class token_e {
  FN,
  ARROW,
  ASM,
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
  COLON,
  L_BRACE,
  R_BRACE,

  BITWISE_AND,
  BITWISE_OR,
  BITWISE_XOR,
  BITWISE_NOT,
  LSH,
  RSH,
  L_BRACKET,
  R_BRACKET,
  MOD,

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

} // namespace front
} // namespace compiler

#endif