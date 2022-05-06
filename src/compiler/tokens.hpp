#ifndef COMPILER_TOKENS_HPP
#define COMPILER_TOKENS_HPP

#include <string>
#include "ast.hpp"

namespace compiler {

enum class token_e {
  DATA,
  DIM,
  ID,
  END,
  FOR,
  TO,
  STEP,
  GOTO,
  GOSUB,
  IF,
  THEN,
  INPUT,
  LET,
  NEXT,
  OPEN,
  POKE,
  PRINT,
  OCTOTHORP,
  READ,
  RETURN,
  REM,
  NOT,
  AND,
  OR,
  COMMA,
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