#include "compiler/front/lexer.hpp"
#include "compiler/front/tokens.hpp"

#include <string>
#include <vector>

#include <CppUTest/TestHarness.h>

namespace {
struct test_case {
  std::string input;
  std::vector<compiler::front::token_e> expected;
};
} // namespace

TEST_GROUP(lexer_tests){};

TEST(lexer_tests, all) {
  std::vector<test_case> tcs = {
      {"asm", {compiler::front::token_e::ASM}},
      {"x", {compiler::front::token_e::ID}},
      {"for", {compiler::front::token_e::FOR}},
      {"to", {compiler::front::token_e::TO}},
      {"step", {compiler::front::token_e::STEP}},
      {"goto", {compiler::front::token_e::GOTO}},
      {"gosub", {compiler::front::token_e::GOSUB}},
      {"if", {compiler::front::token_e::IF}},
      {"then", {compiler::front::token_e::THEN}},
      {"else", {compiler::front::token_e::ELSE}},
      {"elif", {compiler::front::token_e::ELIF}},
      {"let", {compiler::front::token_e::LET}},
      {"#", {compiler::front::token_e::OCTOTHORP}},
      {"read", {compiler::front::token_e::READ}},
      {"return", {compiler::front::token_e::RETURN}},
      {"!", {compiler::front::token_e::NOT}},
      {"&&", {compiler::front::token_e::AND}},
      {"||", {compiler::front::token_e::OR}},
      {",", {compiler::front::token_e::COMMA}},
      {"==", {compiler::front::token_e::EQ_EQ}},
      {"!=", {compiler::front::token_e::NOT_EQ}},
      {">", {compiler::front::token_e::GT}},
      {">=", {compiler::front::token_e::GTE}},
      {"<", {compiler::front::token_e::LT}},
      {"<=", {compiler::front::token_e::LTE}},
      {"+", {compiler::front::token_e::ADD}},
      {"-", {compiler::front::token_e::SUB}},
      {"/", {compiler::front::token_e::DIV}},
      {"*", {compiler::front::token_e::MUL}},
      {"^", {compiler::front::token_e::HAT}},
      {"(", {compiler::front::token_e::L_PAREN}},
      {")", {compiler::front::token_e::R_PAREN}},
      {"4194", {compiler::front::token_e::INTEGER}},
      {"2.14159", {compiler::front::token_e::FLOAT}},
      {"'a_program_label", {compiler::front::token_e::LABEL}},
      {";", {compiler::front::token_e::SEMICOLON}},
      {"=", {compiler::front::token_e::EQ}},
      {"{", {compiler::front::token_e::L_BRACE}},
      {"}", {compiler::front::token_e::R_BRACE}},
      {":", {compiler::front::token_e::COLON}}

  };

  for (auto &tc : tcs) {
    compiler::front::lexer_c lexer;
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