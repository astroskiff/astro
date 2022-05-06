#include "tokens.hpp"

#include "ast.hpp"
#include <string>

namespace compiler {
std::string token_to_str(const td_pair_t &td) {
  auto show = [](const std::string &data, const location_c loc) -> std::string {
    return data + "[" + std::to_string(loc.line) + ", " +
           std::to_string(loc.col) + "]";
  };
  switch (td.token) {
  case token_e::DATA:
    return show("DATA", td.location);
  case token_e::DIM:
    return show("DIM", td.location);
  case token_e::ID:
    return show("ID", td.location);
  case token_e::END:
    return show("END", td.location);
  case token_e::FOR:
    return show("FOR", td.location);
  case token_e::TO:
    return show("TO", td.location);
  case token_e::STEP:
    return show("STEP", td.location);
  case token_e::GOTO:
    return show("GOTO", td.location);
  case token_e::GOSUB:
    return show("GOSUB", td.location);
  case token_e::IF:
    return show("IF", td.location);
  case token_e::THEN:
    return show("THEN", td.location);
  case token_e::INPUT:
    return show("INPUT", td.location);
  case token_e::LET:
    return show("LET", td.location);
  case token_e::NEXT:
    return show("NEXT", td.location);
  case token_e::OPEN:
    return show("OPEN", td.location);
  case token_e::POKE:
    return show("POKE", td.location);
  case token_e::PRINT:
    return show("PRINT", td.location);
  case token_e::OCTOTHORP:
    return show("OCTOTHORP", td.location);
  case token_e::READ:
    return show("READ", td.location);
  case token_e::REM:
    return show("REM", td.location);
  case token_e::RETURN:
    return show("RETURN", td.location);
  case token_e::NOT:
    return show("NOT", td.location);
  case token_e::AND:
    return show("AND", td.location);
  case token_e::OR:
    return show("OR", td.location);
  case token_e::COMMA:
    return show("COMMA", td.location);
  case token_e::EQ:
    return show("EQ", td.location);
  case token_e::EQ_EQ:
    return show("EQ_EQ", td.location);
  case token_e::NOT_EQ:
    return show("NOT_EQ", td.location);
  case token_e::GT:
    return show("GT", td.location);
  case token_e::GTE:
    return show("GTE", td.location);
  case token_e::LT:
    return show("LT", td.location);
  case token_e::LTE:
    return show("LTE", td.location);
  case token_e::ADD:
    return show("ADD", td.location);
  case token_e::SUB:
    return show("SUB", td.location);
  case token_e::DIV:
    return show("DIV", td.location);
  case token_e::MUL:
    return show("MUL", td.location);
  case token_e::HAT:
    return show("HAT", td.location);
  case token_e::L_PAREN:
    return show("L_PAREN", td.location);
  case token_e::R_PAREN:
    return show("R_PAREN", td.location);
  case token_e::INTEGER:
    return show("INTEGER", td.location);
  case token_e::FLOAT:
    return show("FLOAT", td.location);
  case token_e::STRING:
    return show("STRING", td.location);
  case token_e::LABEL:
    return show("LABEL", td.location);
  case token_e::SEMICOLON:
    return show("SEMICOLON", td.location);
  case token_e::ERT:
    return "ERT";
  case token_e::EOS:
    return "EOS";
  }
  return show("UNKNOWN", {0, 0});
}
} // namespace compiler