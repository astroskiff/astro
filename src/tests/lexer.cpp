#include "compiler/lexer.hpp"
#include "compiler/tokens.hpp"

#include <string>
#include <vector>

#include <CppUTest/TestHarness.h>

namespace {
struct test_case {
  std::string input;
  std::vector<compiler::token_e> expected;
};
} // namespace

TEST_GROUP(lexer_tests){};

TEST(lexer_tests, all) {
  std::vector<test_case> tcs = {
      {"fn", {compiler::token_e::FN}},
      {"some_variable", {compiler::token_e::IDENTIFIER}},
      {"(", {compiler::token_e::L_PAREN}},
      {")", {compiler::token_e::R_PAREN}},
      {"{", {compiler::token_e::L_BRACE}},
      {"}", {compiler::token_e::R_BRACE}},
      {"[", {compiler::token_e::L_BRACKET}},
      {"]", {compiler::token_e::R_BRACKET}},
      {":", {compiler::token_e::COLON}},
      {";", {compiler::token_e::SEMICOLON}},
      {",", {compiler::token_e::COMMA}},
      {">", {compiler::token_e::GT}},
      {"<", {compiler::token_e::LT}},
      {"<=", {compiler::token_e::LTE}},
      {">=", {compiler::token_e::GTE}},
      {"->", {compiler::token_e::ARROW}},
      {"+", {compiler::token_e::ADD}},
      {"-", {compiler::token_e::SUB}},
      {"*", {compiler::token_e::MUL}},
      {"/", {compiler::token_e::DIV}},
      {"%", {compiler::token_e::MOD}},
      {"**", {compiler::token_e::POW}},
      {"+=", {compiler::token_e::ADD_EQ}},
      {"-=", {compiler::token_e::SUB_EQ}},
      {"*=", {compiler::token_e::MUL_EQ}},
      {"/=", {compiler::token_e::DIV_EQ}},
      {"%=", {compiler::token_e::MOD_EQ}},
      {"**=", {compiler::token_e::POW_EQ}},
      {"&", {compiler::token_e::AMPERSAND}},
      {"|", {compiler::token_e::PIPE}},
      {"~", {compiler::token_e::TILDE}},
      {"^", {compiler::token_e::HAT}},
      {"&=", {compiler::token_e::AMPERSAND_EQ}},
      {"|=", {compiler::token_e::PIPE_EQ}},
      {"~=", {compiler::token_e::TILDE_EQ}},
      {"^=", {compiler::token_e::HAT_EQ}},
      {"=", {compiler::token_e::EQ}},
      {"==", {compiler::token_e::EQ_EQ}},
      {"@", {compiler::token_e::AT}},
      {"$", {compiler::token_e::DOLLAR}},
      {"\"Some string\"", {compiler::token_e::STRING}},
      {"'", {compiler::token_e::SINGLE_QUOTE}},
      {"?", {compiler::token_e::QUESTION_MARK}},
      {".", {compiler::token_e::PERIOD}},
      {"#", {compiler::token_e::OCTOTHORPE}},
      {"3.14159", {compiler::token_e::LITERAL_FLOAT}},
      {"1945", {compiler::token_e::LITERAL_NUMBER}},
      {"||", {compiler::token_e::OR}},
      {"&&", {compiler::token_e::AND}},
      {"!", {compiler::token_e::EXCLAMATION}},
      {"!=", {compiler::token_e::EXCLAMATION_EQ}},
      {"while", {compiler::token_e::WHILE}},
      {"for", {compiler::token_e::FOR}},
      {"if", {compiler::token_e::IF}},
      {"else", {compiler::token_e::ELSE}},
      {"return", {compiler::token_e::RETURN}},
      {"break", {compiler::token_e::BREAK}},
      {"let", {compiler::token_e::LET}},
      {"use", {compiler::token_e::USE}},
      {"import", {compiler::token_e::IMPORT}},
      {"<<", {compiler::token_e::LSH}},
      {">>", {compiler::token_e::RSH}},
      {"<<=", {compiler::token_e::LSH_EQ}},
      {">>=", {compiler::token_e::RSH_EQ}},
      {"struct", {compiler::token_e::STRUCT}},
      {"container", {compiler::token_e::CONTAINER}},
      {"fixed", {compiler::token_e::C_FIXED}},
      {"variable", {compiler::token_e::C_VARIABLE}},
      {"pub", {compiler::token_e::PUB}},
  };

  for (auto &tc : tcs) {
    compiler::lexer_c lexer;
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