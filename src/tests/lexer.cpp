#include "compiler/lang/lexer.hpp"
#include "compiler/lang/tokens.hpp"

#include <string>
#include <vector>

#include <CppUTest/TestHarness.h>

namespace {
struct test_case {
  std::string input;
  std::vector<compiler::lang::token_e> expected;
};
} // namespace

TEST_GROUP(lexer_tests){};

TEST(lexer_tests, all) {
  std::vector<test_case> tcs = {
      {"fn", {compiler::lang::token_e::FN}},
      {"some_variable", {compiler::lang::token_e::IDENTIFIER}},
      {"(", {compiler::lang::token_e::L_PAREN}},
      {")", {compiler::lang::token_e::R_PAREN}},
      {"{", {compiler::lang::token_e::L_BRACE}},
      {"}", {compiler::lang::token_e::R_BRACE}},
      {"[", {compiler::lang::token_e::L_BRACKET}},
      {"]", {compiler::lang::token_e::R_BRACKET}},
      {":", {compiler::lang::token_e::COLON}},
      {";", {compiler::lang::token_e::SEMICOLON}},
      {",", {compiler::lang::token_e::COMMA}},
      {">", {compiler::lang::token_e::GT}},
      {"<", {compiler::lang::token_e::LT}},
      {"<=", {compiler::lang::token_e::LTE}},
      {">=", {compiler::lang::token_e::GTE}},
      {"->", {compiler::lang::token_e::ARROW}},
      {"+", {compiler::lang::token_e::ADD}},
      {"-", {compiler::lang::token_e::SUB}},
      {"*", {compiler::lang::token_e::MUL}},
      {"/", {compiler::lang::token_e::DIV}},
      {"%", {compiler::lang::token_e::MOD}},
      {"**", {compiler::lang::token_e::POW}},
      {"+=", {compiler::lang::token_e::ADD_EQ}},
      {"-=", {compiler::lang::token_e::SUB_EQ}},
      {"*=", {compiler::lang::token_e::MUL_EQ}},
      {"/=", {compiler::lang::token_e::DIV_EQ}},
      {"%=", {compiler::lang::token_e::MOD_EQ}},
      {"**=", {compiler::lang::token_e::POW_EQ}},
      {"&", {compiler::lang::token_e::AMPERSAND}},
      {"|", {compiler::lang::token_e::PIPE}},
      {"~", {compiler::lang::token_e::TILDE}},
      {"^", {compiler::lang::token_e::HAT}},
      {"&=", {compiler::lang::token_e::AMPERSAND_EQ}},
      {"|=", {compiler::lang::token_e::PIPE_EQ}},
      {"~=", {compiler::lang::token_e::TILDE_EQ}},
      {"^=", {compiler::lang::token_e::HAT_EQ}},
      {"=", {compiler::lang::token_e::EQ}},
      {"==", {compiler::lang::token_e::EQ_EQ}},
      {"@", {compiler::lang::token_e::AT}},
      {"$", {compiler::lang::token_e::DOLLAR}},
      {"\"Some string\"", {compiler::lang::token_e::STRING}},
      {"'", {compiler::lang::token_e::SINGLE_QUOTE}},
      {"?", {compiler::lang::token_e::QUESTION_MARK}},
      {".", {compiler::lang::token_e::PERIOD}},
      {"#", {compiler::lang::token_e::OCTOTHORPE}},
      {"3.14159", {compiler::lang::token_e::LITERAL_FLOAT}},
      {"1945", {compiler::lang::token_e::LITERAL_NUMBER}},
      {"||", {compiler::lang::token_e::OR}},
      {"&&", {compiler::lang::token_e::AND}},
      {"!", {compiler::lang::token_e::EXCLAMATION}},
      {"!=", {compiler::lang::token_e::EXCLAMATION_EQ}},
      {"while", {compiler::lang::token_e::WHILE}},
      {"for", {compiler::lang::token_e::FOR}},
      {"if", {compiler::lang::token_e::IF}},
      {"else", {compiler::lang::token_e::ELSE}},
      {"return", {compiler::lang::token_e::RETURN}},
      {"break", {compiler::lang::token_e::BREAK}},
      {"let", {compiler::lang::token_e::LET}},
      {"use", {compiler::lang::token_e::USE}},
      {"import", {compiler::lang::token_e::IMPORT}},
      {"<<", {compiler::lang::token_e::LSH}},
      {">>", {compiler::lang::token_e::RSH}},
      {"<<=", {compiler::lang::token_e::LSH_EQ}},
      {">>=", {compiler::lang::token_e::RSH_EQ}},
      {"struct", {compiler::lang::token_e::STRUCT}},
      {"container", {compiler::lang::token_e::CONTAINER}},
      {"fixed", {compiler::lang::token_e::C_FIXED}},
      {"variable", {compiler::lang::token_e::C_VARIABLE}},
      {"pub", {compiler::lang::token_e::PUB}},
  };

  for (auto &tc : tcs) {
    compiler::lang::lexer_c lexer;
    auto result = lexer.lex(0, tc.input);
    CHECK_EQUAL_TEXT(tc.expected.size(), result.size(),
                     "Size of resulting vector unexpected");
    for (size_t i = 0; i < tc.expected.size(); i++) {
      CHECK_EQUAL_TEXT(
          static_cast<int>(tc.expected[i]), static_cast<int>(result[i].token),
          std::string("Unexpected resulting token from : " + tc.input).c_str());
    }
  }
}