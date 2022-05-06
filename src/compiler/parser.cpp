#include "parser.hpp"
#include "tokens.hpp"
#include "lexer.hpp"
#include "page.hpp"
#include <string>
#include <limits>
#include <iostream>

namespace compiler {

namespace {

std::vector<td_pair_t> read_and_lex_file(const std::string &file) {

  std::vector<td_pair_t> result;
  page_c file_data;
  if (!file_data.load_page(file)) {
    std::cerr << "Unable to load file : " << file << std::endl;
    return {};
  }

  for (std::size_t line = 0; line < file_data.get_page_size();
       line++) {

    lexer_c lexer;
    auto [okay, line_data] = file_data.get_line(line);
    if (!okay) {
      // Shouldn't happen given our means of iterating
      std::cerr << "Internal Error : Ran over page data" << std::endl;
      return {};
    }

    auto tokens = lexer.lex(line, line_data);
    result.insert(result.end(), tokens.begin(), tokens.end());
  }
  return result;
}

td_pair_t error_token = {token_e::ERT, {}, 0};
td_pair_t end_of_stream = {token_e::EOS, {}, 0};

std::unordered_map<token_e, parser_c::precedence_e> precedences = {
    {token_e::EQ, parser_c::precedence_e::ASSIGN},
    {token_e::EQ_EQ, parser_c::precedence_e::EQUALS},
    {token_e::ADD_EQ, parser_c::precedence_e::EQUALS},
    {token_e::SUB_EQ, parser_c::precedence_e::EQUALS},
    {token_e::DIV_EQ, parser_c::precedence_e::EQUALS},
    {token_e::MUL_EQ, parser_c::precedence_e::EQUALS},
    {token_e::MOD_EQ, parser_c::precedence_e::EQUALS},
    {token_e::POW_EQ, parser_c::precedence_e::EQUALS},
    {token_e::LSH_EQ, parser_c::precedence_e::EQUALS},
    {token_e::RSH_EQ, parser_c::precedence_e::EQUALS},
    {token_e::HAT_EQ, parser_c::precedence_e::EQUALS},
    {token_e::PIPE_EQ, parser_c::precedence_e::EQUALS},
    {token_e::TILDE_EQ, parser_c::precedence_e::EQUALS},
    {token_e::AMPERSAND_EQ, parser_c::precedence_e::EQUALS},
    {token_e::EXCLAMATION_EQ, parser_c::precedence_e::EQUALS},
    {token_e::LT, parser_c::precedence_e::LESS_GREATER},
    {token_e::GT, parser_c::precedence_e::LESS_GREATER},
    {token_e::LTE, parser_c::precedence_e::LESS_GREATER},
    {token_e::GTE, parser_c::precedence_e::LESS_GREATER},
    {token_e::RSH, parser_c::precedence_e::SHIFT},
    {token_e::LSH, parser_c::precedence_e::SHIFT},
    {token_e::ADD, parser_c::precedence_e::SUM},
    {token_e::SUB, parser_c::precedence_e::SUM},
    {token_e::DIV, parser_c::precedence_e::PROD},
    {token_e::MUL, parser_c::precedence_e::PROD},
    {token_e::MOD, parser_c::precedence_e::PROD},
    {token_e::POW, parser_c::precedence_e::POW},
    {token_e::AMPERSAND, parser_c::precedence_e::BITWISE},
    {token_e::HAT, parser_c::precedence_e::BITWISE},
    {token_e::TILDE, parser_c::precedence_e::BITWISE},
    {token_e::OR, parser_c::precedence_e::LOGICAL},
    {token_e::AND, parser_c::precedence_e::LOGICAL},
    {token_e::PIPE, parser_c::precedence_e::LOGICAL},
    {token_e::L_PAREN, parser_c::precedence_e::CALL},
    {token_e::L_BRACKET, parser_c::precedence_e::INDEX},
};

}

parser_c::parser_c()
{

}

void parser_c::prev() { _idx--; }

void parser_c::advance() { _idx++; }

void parser_c::mark() { _mark = _idx; }

void parser_c::unset() { _mark = std::numeric_limits<uint64_t>::max(); }

const td_pair_t &parser_c::current_td_pair() const
{
  if (_idx >= _tokens.size()) {
    return error_token;
  }

  return _tokens.at(_idx);
}

void parser_c::reset()
{
  if (_mark > _idx) {
    //_err.raise(error::parser::INTERNAL_MARK_UNSET);
    _parser_okay = false;
    unset(); // ensure its set to max, not some other num
    return;
  }

  while (_idx != _mark) {
    prev();
  }

  unset();
}

const td_pair_t &parser_c::peek(const std::size_t ahead) const
{
  if (_idx + ahead >= _tokens.size()) {
    return end_of_stream;
  }
  return _tokens.at(_idx + ahead);
}

void parser_c::die(uint64_t error_no, std::string error)
{
  //  Todo : Send this data to the reporter
  //
  std::cerr << "Error: " << error_no << " | " << error << " .. " 
            << current_td_pair().line << ", "<< current_td_pair().col << std::endl;

  std::cerr << "Current token : " << token_to_str(current_td_pair());
}

void parser_c::expect(const token_e token, const std::string &error, const size_t ahead)
{
  if (peek(ahead).token != token) {
    die(0, error); // Todo: Update this error code to "unexpected token" code
  }
}

parser_c::precedence_e parser_c::peek_precedence()
{
  if (precedences.find(peek().token) != precedences.end()) {
    return precedences[peek().token];
  }
  return parser_c::precedence_e::LOWEST;
}

std::vector<node_c*> parser_c::parse_file(const std::string &file)
{
  _idx = 0;
  _mark = 0;
  _parser_okay = true;
  _source_name = file;

  //_prefix_fns[token_e::IDENTIFIER] = &parser_c::identifier;
  //_prefix_fns[token_e::LITERAL_NUMBER] = &parser_c::number;
  //_prefix_fns[token_e::LITERAL_FLOAT] = &parser_c::number;
  //_prefix_fns[token_e::STRING] = &parser_c::str;
  //_prefix_fns[token_e::EXCLAMATION] = &parser_c::prefix_expr;
  //_prefix_fns[token_e::TILDE] = &parser_c::prefix_expr;
  //_prefix_fns[token_e::SUB] = &parser_c::prefix_expr;
  //_prefix_fns[token_e::L_PAREN] = &parser_c::grouped_expr;
  //_prefix_fns[token_e::L_BRACE] = &parser_c::array;

  //_infix_fns[token_e::ADD] = &parser_c::infix_expr;
  //_infix_fns[token_e::SUB] = &parser_c::infix_expr;
  //_infix_fns[token_e::DIV] = &parser_c::infix_expr;
  //_infix_fns[token_e::MUL] = &parser_c::infix_expr;
  //_infix_fns[token_e::MOD] = &parser_c::infix_expr;
  //_infix_fns[token_e::EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::EQ_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::ADD_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::SUB_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::DIV_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::MUL_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::MOD_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::POW_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::AMPERSAND_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::HAT_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::PIPE_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::TILDE_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::LSH_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::RSH_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::EXCLAMATION_EQ] = &parser_c::infix_expr;
  //_infix_fns[token_e::LT] = &parser_c::infix_expr;
  //_infix_fns[token_e::LTE] = &parser_c::infix_expr;
  //_infix_fns[token_e::GT] = &parser_c::infix_expr;
  //_infix_fns[token_e::GTE] = &parser_c::infix_expr;
  //_infix_fns[token_e::RSH] = &parser_c::infix_expr;
  //_infix_fns[token_e::LSH] = &parser_c::infix_expr;
  //_infix_fns[token_e::POW] = &parser_c::infix_expr;
  //_infix_fns[token_e::OR] = &parser_c::infix_expr;
  //_infix_fns[token_e::AND] = &parser_c::infix_expr;
  //_infix_fns[token_e::PIPE] = &parser_c::infix_expr;
  //_infix_fns[token_e::HAT] = &parser_c::infix_expr;
  //_infix_fns[token_e::AMPERSAND] = &parser_c::infix_expr;
  //_infix_fns[token_e::L_PAREN] = &parser_c::call_expr;
  //_infix_fns[token_e::L_BRACKET] = &parser_c::index_expr;

  _tokens = read_and_lex_file(file);

  if (_tokens.empty()) {
    return {};
  }

  std::vector<node_c*> results;
  while (_parser_okay && _idx < _tokens.size()) 
  {
    //  Check for top level items
    //
    auto item = top_level_item();
    if (!item) {
      die(0, "Invalid item"); // TODO: Update this error code
      return {};
    }

    //  Add top level items
    //
    results.push_back(std::move(item));
  }

  _tokens.clear();

  if (!_parser_okay) {
    results.clear();
  }

  return results;
}

node_c* parser_c::top_level_item()
{
  // Check for 'function' statement
  // Check for 'struct' statement
  // Check for 'container' statement

  return nullptr;
}

}