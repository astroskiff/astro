#include "parser.hpp"
#include "tokens.hpp"
#include "lexer.hpp"
#include <string>
#include <limits>

#include "external/aixlog.hpp"
#include "defines.hpp"

#include <iostream>

namespace compiler {
namespace lang {

namespace {

std::vector<lang::td_pair_t> read_and_lex_file(const std::string &file,
                                               lang::compiler_data_t &cdt) {

  std::vector<lang::td_pair_t> result;
  source::page_c file_data;
  if (!file_data.load_page(file)) {
    std::cerr << "Unable to load file : " << file << std::endl;
    return {};
  }
  cdt.source_code[file] = std::move(file_data);

  for (std::size_t line = 0; line < cdt.source_code[file].get_page_size();
       line++) {

    lexer_c lexer;
    auto [okay, line_data] = cdt.source_code[file].get_line(line);
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

lang::td_pair_t error_token = {token_e::ERT, {}, 0};
lang::td_pair_t end_of_stream = {token_e::EOS, {}, 0};

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

parser_c::parser_c(compiler_data_t &cdt) : _cdt(cdt)
{

}

void parser_c::prev() { _idx--; }

void parser_c::advance() { _idx++; }

void parser_c::mark() { _mark = _idx; }

void parser_c::unset() { _mark = std::numeric_limits<uint64_t>::max(); }

const lang::td_pair_t &parser_c::current_td_pair() const
{
  if (_idx >= _tokens.size()) {
    LOG(DEBUG) << TAG(APP_FILE_NAME) << "[" << APP_LINE
               << "]: End of token stream" << std::endl;
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

const lang::td_pair_t &parser_c::peek(const std::size_t ahead) const
{
  if (_idx + ahead >= _tokens.size()) {
    LOG(DEBUG) << TAG(APP_FILE_NAME) << "[" << APP_LINE
                << "]: End of token stream" << std::endl;
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

std::vector<instruction_ptr> parser_c::parse_file(const std::string &file)
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

  _tokens = read_and_lex_file(file, _cdt);

  if (_tokens.empty()) {
    return {};
  }

  std::vector<instruction_ptr> results;
  while (_parser_okay && _idx < _tokens.size()) 
  {
    //  Check for use statement and pull in instructions from module
    //
    auto use_stmt = use_statement();
    if (!use_stmt.empty()) {
      results.insert(results.end(), 
      std::make_move_iterator(use_stmt.begin()), 
      std::make_move_iterator(use_stmt.end()));
    }

    //  Check for import statement and pull in instructions from file
    //
    auto import_stmt = import_statement();
    if (!import_stmt.empty()) {
      results.insert(results.end(), 
      std::make_move_iterator(import_stmt.begin()), 
      std::make_move_iterator(import_stmt.end()));
    }

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

std::vector<lang::instruction_ptr> parser_c::import_statement()
{
  std::vector<lang::instruction_ptr> result;


  return result;
}

std::vector<lang::instruction_ptr> parser_c::use_statement()
{
  if (current_td_pair().token != token_e::USE) {
    return {};
  }

  //  modules are not allowed to be use
  //
  if (_parsing_module) {
    /*
      TODO: Modules should be permitted to pull in modules external to the project, or 
      from a standard library of modules. 

      Something like : 

        use local::module_name; 

        could be used to indicate that a local (to the app) module is to be improted

        and 

        use std::module_name;

        could be used to indicate that we want a standard library module or something, but the
        project info will have to be updated to give us a path to module locations 
        so we can resolve the paths for modules external to the project
    
    */
    std::cerr << "Modules are not currently permitted to `use` other modules" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  lang::location_c location(current_td_pair().line, current_td_pair().col);
  
  advance();
  expect(token_e::IDENTIFIER, "Expected module name");

  /*
      TODO: 

        check for  `::` to see if we are looking at a "local::module_name" or a "std::module_name" 

      
  
  */



  std::string module_name = current_td_pair().data;

  auto module_iterator = std::find(_cdt.known_modules.begin(), _cdt.known_modules.end(), module_name);

  if (module_iterator == _cdt.known_modules.end()) {
    die(0, "Unknown module : " + module_name);  // TODO: Update this to unknown module error code
    return {};
  }

  std::filesystem::path path_to_module = _cdt.operating_directory.top().parent_path();
  path_to_module /= module_name;

  if (!std::filesystem::is_directory(path_to_module)) {
    die(0, "Module not found : " + module_name);  // TODO: Update this to module not found (no dir)
    return {};
  }

  auto operating_directory = path_to_module;
  path_to_module /= "mod.ast";

  if (!std::filesystem::is_regular_file(path_to_module)) {
    die(0, "Module does not contain a 'mod.ast' file");  // TODO : Update this to file not found (no mod.ast)
    return {};
  }

  std::vector<lang::instruction_ptr> result;

  {
    // Indicate that the operating directory is the module
    _cdt.operating_directory.push(operating_directory);

    // Create the module parser
    parser_c module_parser(_cdt);

    // Indicate to the parser that it is operating within a module
    module_parser.mark_parsing_module();

    // Parse the mod.ast file
    result = module_parser.parse_file(path_to_module.string());

    // Indicate that we are now operating back in the current directory
    _cdt.operating_directory.pop();

    // Check to see if the module parser resulted in success
    if (!module_parser.is_okay()) {
      die(0, "Failed to parse module : " + module_name);
      return {};
    }
  }

  advance(); // go past module name

  expect(token_e::SEMICOLON, "Expected semicolon");

  advance(); // go past semicolon

  return result;
}

lang::instruction_ptr parser_c::top_level_item()
{
  // Check for 'function' statement
  // Check for 'struct' statement
  // Check for 'container' statement

  return nullptr;
}

}
}