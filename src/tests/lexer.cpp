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
      {"x", {compiler::token_e::ID}},
      {"for", {compiler::token_e::FOR}},
      {"to", {compiler::token_e::TO}},
      {"step", {compiler::token_e::STEP}},
      {"goto", {compiler::token_e::GOTO}},
      {"gosub", {compiler::token_e::GOSUB}},
      {"if", {compiler::token_e::IF}},
      {"then", {compiler::token_e::THEN}},
      {"else", {compiler::token_e::ELSE}},
      {"elif", {compiler::token_e::ELIF}},
      {"let", {compiler::token_e::LET}},
      {"#", {compiler::token_e::OCTOTHORP}},
      {"read", {compiler::token_e::READ}},
      {"return", {compiler::token_e::RETURN}},
      {"!", {compiler::token_e::NOT}},
      {"&&", {compiler::token_e::AND}},
      {"||", {compiler::token_e::OR}},
      {",", {compiler::token_e::COMMA}},
      {"==", {compiler::token_e::EQ_EQ}},
      {"!=", {compiler::token_e::NOT_EQ}},
      {">", {compiler::token_e::GT}},
      {">=", {compiler::token_e::GTE}},
      {"<", {compiler::token_e::LT}},
      {"<=", {compiler::token_e::LTE}},
      {"+", {compiler::token_e::ADD}},
      {"-", {compiler::token_e::SUB}},
      {"/", {compiler::token_e::DIV}},
      {"*", {compiler::token_e::MUL}},
      {"^", {compiler::token_e::HAT}},
      {"(", {compiler::token_e::L_PAREN}},
      {")", {compiler::token_e::R_PAREN}},
      {"4194", {compiler::token_e::INTEGER}},
      {"2.14159", {compiler::token_e::FLOAT}},
      {"'a_program_label", {compiler::token_e::LABEL}},
      {";", {compiler::token_e::SEMICOLON}},
      {"=", {compiler::token_e::EQ}},
      {"{", {compiler::token_e::L_BRACE}},
      {"}", {compiler::token_e::R_BRACE}},
      {":", {compiler::token_e::COLON}}

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