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

td_pair_t error_token = {token_e::ERT, {}, {0,0}};
td_pair_t end_of_stream = {token_e::EOS, {}, {0,0}};

}

parser_c::parser_c()
{
  _statement_functions = {
    std::bind(&parser_c::let_statement, this)
  };
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
            << current_td_pair().location.line << ", "<< current_td_pair().location.col << std::endl;

  std::cerr << "Current token : " << token_to_str(current_td_pair());
  _parser_okay = false;
}

void parser_c::expect(const token_e token, const std::string &error, const size_t ahead)
{
  if (peek(ahead).token != token) {
    die(0, error); // Todo: Update this error code to "unexpected token" code
  }
}

std::vector<node_c*> parser_c::parse_file(const std::string &file)
{
  _idx = 0;
  _mark = 0;
  _parser_okay = true;
  _source_name = file;

  _tokens = read_and_lex_file(file);

  if (_tokens.empty()) {
    return {};
  }

  std::vector<node_c*> results;
  while (_parser_okay && _idx < _tokens.size()) 
  {
    //  Check for top level items
    //
    auto item = statement();
    if (!item) {
      die(0, "Invalid statement"); // TODO: Update this error code
      return {};
    }

    //  Add top level items
    //
    results.push_back(item);
  }

  _tokens.clear();

  if (!_parser_okay) {
    results.clear();
  }

  return results;
}

node_c* parser_c::statement()
{
  for( auto &&func : _statement_functions) {
    if (!_parser_okay) {
      return nullptr;
    }

    mark();
    if (auto result = func(); result) {
      return result;
    } 
    reset();
  }
  return nullptr;
}

node_c* parser_c::let_statement()
{
  if(current_td_pair().token != token_e::LET) {
    return nullptr;
  }
  auto location = current_td_pair().location;

  advance();

  if(current_td_pair().token != token_e::ID) {
    die(0, "Expected ID");
    return nullptr;
  }
  auto id = current_td_pair().data;
  auto id_loc = current_td_pair().location;

  advance();

  if(current_td_pair().token != token_e::EQ) {
    die(0, "Expected '=' for assignment'");
    return nullptr;
  }

  advance();

  auto exp = expression();
  if (!exp) {
    die(0, "Invalid expression");
    return nullptr;
  }

  if(current_td_pair().token != token_e::SEMICOLON) {
    die(0, "Missing ';'");
    return nullptr;
  }
  advance();

  auto assignment_node = new node_c(node_type::LET, location, "let");
  auto variable_node = new node_c(node_type::ID, id_loc, id);
  append_node(assignment_node, variable_node);
  append_node(assignment_node, exp);
  return assignment_node;
}


node_c* parser_c::expression() { 
  
  auto current_exp = and_exp();
  if (!current_exp) {
    return nullptr;
  }  

  if (current_td_pair().token == token_e::OR) {

    auto or_node = new node_c(node_type::OR, current_td_pair().location);
    append_node(or_node, current_exp);

    auto next_exp = expression();
    if (!next_exp) {
      free_nodes(or_node);
      return nullptr;
    }
    append_node(or_node, next_exp);
    return or_node;
  }

  return current_exp;
}

node_c* parser_c::and_exp() 
{ 
  auto current_exp = not_exp();
  if (!current_exp) {
    return nullptr;
  }  

  if (current_td_pair().token == token_e::AND) {

    auto and_node = new node_c(node_type::AND, current_td_pair().location);
    append_node(and_node, current_exp);

    auto next_exp = and_exp();
    if (!next_exp) {
      free_nodes(and_node);
      return nullptr;
    }
    append_node(and_node, next_exp);
    return and_node;
  }

  return current_exp;
}

node_c* parser_c::not_exp() 
{ 
  if (current_td_pair().token == token_e::NOT) {
    advance();
    auto not_node = new node_c(node_type::NOT, current_td_pair().location);
    auto expr = compare_exp();
    if(!expr) {
      free_nodes(not_node);
      return nullptr;
    }
    append_node(not_node, expr);
    return not_node;
  }
  return compare_exp();
}


node_c* parser_c::compare_exp() 
{ 
  return add_exp(); 
}

node_c* parser_c::add_exp() 
{ 
  auto current_expression = mult_exp();
  if (!current_expression) {
    return nullptr;
  }

  std::cout << token_to_str(current_td_pair()) << std::endl;

  if (current_td_pair().token == token_e::ADD) {
    auto op_node = new node_c(node_type::ADD, current_td_pair().location, "+");
    advance();
    auto next_expression = add_exp();
    if (!next_expression) {
      delete op_node;
      return nullptr;
    }
  return current_expression; 
  }

  if (current_td_pair().token == token_e::SUB) {
    auto op_node = new node_c(node_type::SUB, current_td_pair().location, "-");
    advance();

    auto next_expression = add_exp();
    if (!next_expression) {
      delete op_node;
      return nullptr;
    }
    return current_expression; 

  }

  return current_expression; 
}

node_c* parser_c::mult_exp() 
{ 
  return negate_exp();
}

node_c* parser_c::negate_exp() 
{ 
  return power_exp();
}
node_c* parser_c::power_exp()
{ 
  return value(); 
}

node_c* parser_c::value() 
{ 
  /*
  if (current_td_pair().token != token_e::ID) {
    die(0, "Expected id");
    return nullptr;
  }
  auto id_node = new node_c(node_type::ID, current_td_pair().location);
  advance();
  return id_node;
  */
  return constant();
}

node_c * parser_c::constant()
{
  if (current_td_pair().token != token_e::INTEGER) {
    die(0, "Expected int");
    return nullptr;
  }
  auto data_node = new node_c(node_type::INTEGER, current_td_pair().location);
  data_node->data = current_td_pair().data;
  advance();
  return data_node;
}

}