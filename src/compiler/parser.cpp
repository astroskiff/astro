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