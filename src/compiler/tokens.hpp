#ifndef COMPILER_TOKENS_HPP
#define COMPILER_TOKENS_HPP

#include <string>

namespace compiler {

enum class token_e {
  FN = 0,
  IDENTIFIER,
  L_PAREN,
  R_PAREN,
  L_BRACE,
  R_BRACE,
  L_BRACKET,
  R_BRACKET,
  COLON,
  SEMICOLON,
  COMMA,
  GT,
  LT,
  LTE,
  GTE,
  ARROW,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  POW,
  ADD_EQ,
  SUB_EQ,
  MUL_EQ,
  DIV_EQ,
  MOD_EQ,
  POW_EQ,
  AMPERSAND,
  PIPE,
  TILDE,
  HAT,
  AMPERSAND_EQ,
  PIPE_EQ,
  TILDE_EQ,
  HAT_EQ,
  EQ,
  EQ_EQ,
  AT,
  DOLLAR,
  STRING,
  SINGLE_QUOTE,
  QUESTION_MARK,
  PERIOD,
  OCTOTHORPE,
  LITERAL_FLOAT,
  LITERAL_NUMBER,
  OR,
  AND,
  EXCLAMATION,
  EXCLAMATION_EQ,
  WHILE,
  FOR,
  IF,
  ELSE,
  RETURN,
  BREAK,
  LET,
  USE,
  IMPORT,
  LSH,
  RSH,
  LSH_EQ,
  RSH_EQ,
  STRUCT,
  CONTAINER,
  C_FIXED,
  C_VARIABLE,
  PUB,
  ERT, // Error token
  EOS  // End of stream
};

struct td_pair_t {
  token_e token;
  std::string data;
  size_t line;
  size_t col;
};

static std::string token_to_str(const td_pair_t &td) {
  size_t line_no = td.line;
  switch (td.token) {
  case token_e::FN:
    return "FN[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::IDENTIFIER:
    return "IDENTIFIER(" + td.data + ")[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::L_BRACKET:
    return "L_BRACKET[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::R_BRACKET:
    return "R_BRACKET[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::L_PAREN:
    return "L_PAREN[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::R_PAREN:
    return "R_PAREN[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::L_BRACE:
    return "L_BRACE[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::R_BRACE:
    return "R_BRACE[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::COLON:
    return "COLON[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::SEMICOLON:
    return "SEMICOLON[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::COMMA:
    return "COMMA[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::GT:
    return "GT[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::LT:
    return "LT[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::GTE:
    return "GTE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::LTE:
    return "LTE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::SUB:
    return "SUB[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::ARROW:
    return "ARROW[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::ADD:
    return "ADD[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::MUL:
    return "MUL[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::DIV:
    return "DIV[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::MOD:
    return "MOD[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::POW:
    return "POW[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::ADD_EQ:
    return "ADD_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::SUB_EQ:
    return "SUB_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::MUL_EQ:
    return "MUL_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::DIV_EQ:
    return "DIV_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::MOD_EQ:
    return "MOD_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::POW_EQ:
    return "POW_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::AMPERSAND:
    return "AMPERSAND[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::PIPE:
    return "PIPE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::TILDE:
    return "TILDE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::HAT:
    return "HAT[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::AMPERSAND_EQ:
    return "AMPERSAND_EQ[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::PIPE_EQ:
    return "PIPE_EQ[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::TILDE_EQ:
    return "TILDE_EQ[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::HAT_EQ:
    return "HAT_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::EQ:
    return "EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::EQ_EQ:
    return "EQ_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::AT:
    return "AT[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::DOLLAR:
    return "DOLLAR[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::STRING:
    return "STRING(" + td.data + ")[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::SINGLE_QUOTE:
    return "SINGLE_QUOTE[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::QUESTION_MARK:
    return "QUESTION_MARK[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::PERIOD:
    return "PERIOD[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::OCTOTHORPE:
    return "OCTOTHORPE[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::LITERAL_FLOAT:
    return "FLOAT(" + td.data + ")[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::LITERAL_NUMBER:
    return "NUMBER(" + td.data + ")[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::OR:
    return "OR[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::AND:
    return "AND[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::EXCLAMATION:
    return "EXCLAMATION[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::EXCLAMATION_EQ:
    return "EXCLAMATION_EQ[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::WHILE:
    return "WHILE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::FOR:
    return "FOR[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::IF:
    return "IF[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::ELSE:
    return "ELSE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::RETURN:
    return "RETURN[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::BREAK:
    return "BREAK[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::LET:
    return "LET[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::USE:
    return "USE[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::IMPORT:
    return "IMPORT[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::LSH:
    return "LSH[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::RSH:
    return "RSH[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::LSH_EQ:
    return "LSH_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::RSH_EQ:
    return "RSH_EQ[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::EOS:
    return "EOS[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::STRUCT:
    return "STRUCT[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::CONTAINER:
    return "CONTAINER[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::C_FIXED:
    return "C_FIXED[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::C_VARIABLE:
    return "C_VARIABLE[" + std::to_string(td.line) + ", " +
           std::to_string(td.col) + "]";
  case token_e::PUB:
    return "PUB[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
           "]";
  case token_e::ERT:
    return "ERT";
  }

  return "UNKNOWN[" + std::to_string(td.line) + ", " + std::to_string(td.col) +
         "]";
}

} // namespace compiler

#endif