#include "parser.hpp"
#include "lexer.hpp"
#include "page.hpp"
#include "tokens.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace compiler {

namespace {

std::vector<td_pair_t> read_and_lex_file(const std::string &file) {

  std::vector<td_pair_t> result;
  page_c file_data;
  if (!file_data.load_page(file)) {
    std::cerr << "Unable to load file : " << file << std::endl;
    return {};
  }

  for (std::size_t line = 0; line < file_data.get_page_size(); line++) {

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

td_pair_t error_token = {token_e::ERT, {}, {0, 0}};
td_pair_t end_of_stream = {token_e::EOS, {}, {0, 0}};

std::unordered_map<token_e, parser_c::precedence_e> precedences = {
    {token_e::EQ, parser_c::precedence_e::ASSIGN},
    {token_e::EQ_EQ, parser_c::precedence_e::EQUALS},
    {token_e::LT, parser_c::precedence_e::LESS_GREATER},
    {token_e::GT, parser_c::precedence_e::LESS_GREATER},
    {token_e::LTE, parser_c::precedence_e::LESS_GREATER},
    {token_e::GTE, parser_c::precedence_e::LESS_GREATER},
    {token_e::ADD, parser_c::precedence_e::SUM},
    {token_e::SUB, parser_c::precedence_e::SUM},
    {token_e::DIV, parser_c::precedence_e::PROD},
    {token_e::MUL, parser_c::precedence_e::PROD},
    {token_e::HAT, parser_c::precedence_e::POW},
    {token_e::OR, parser_c::precedence_e::LOGICAL},
    {token_e::AND, parser_c::precedence_e::LOGICAL},
};

} // namespace

parser_c::parser_c() {
  _statement_functions = {std::bind(&parser_c::let_statement, this)};

  _prefix_fns[token_e::ID] = &parser_c::identifier;
  _prefix_fns[token_e::INTEGER] = &parser_c::number;
  _prefix_fns[token_e::FLOAT] = &parser_c::number;
  _prefix_fns[token_e::STRING] = &parser_c::str;
  _prefix_fns[token_e::SUB] = &parser_c::prefix_expr;
  _prefix_fns[token_e::L_PAREN] = &parser_c::grouped_expr;

  _infix_fns[token_e::ADD] = &parser_c::infix_expr;
  _infix_fns[token_e::SUB] = &parser_c::infix_expr;
  _infix_fns[token_e::DIV] = &parser_c::infix_expr;
  _infix_fns[token_e::MUL] = &parser_c::infix_expr;
  _infix_fns[token_e::EQ] = &parser_c::infix_expr;
  _infix_fns[token_e::EQ_EQ] = &parser_c::infix_expr;
  _infix_fns[token_e::LT] = &parser_c::infix_expr;
  _infix_fns[token_e::LTE] = &parser_c::infix_expr;
  _infix_fns[token_e::GT] = &parser_c::infix_expr;
  _infix_fns[token_e::GTE] = &parser_c::infix_expr;
  _infix_fns[token_e::OR] = &parser_c::infix_expr;
  _infix_fns[token_e::AND] = &parser_c::infix_expr;
  _infix_fns[token_e::HAT] = &parser_c::infix_expr;
}

void parser_c::prev() { _idx--; }

void parser_c::advance() { _idx++; }

void parser_c::mark() { _mark = _idx; }

void parser_c::unset() { _mark = std::numeric_limits<uint64_t>::max(); }

const td_pair_t &parser_c::current_td_pair() const {
  if (_idx >= _tokens.size()) {
    return error_token;
  }

  return _tokens.at(_idx);
}

void parser_c::reset() {
  if (_mark > _idx) {
    //_err.raise(error::parser_c::INTERNAL_MARK_UNSET);
    _parser_okay = false;
    unset(); // ensure its set to max, not some other num
    return;
  }

  while (_idx != _mark) {
    prev();
  }

  unset();
}

const td_pair_t &parser_c::peek(const std::size_t ahead) const {
  if (_idx + ahead >= _tokens.size()) {
    return end_of_stream;
  }
  return _tokens.at(_idx + ahead);
}

void parser_c::die(uint64_t error_no, std::string error) {
  //  Todo : Send this data to the reporter
  //
  std::cerr << "Error: " << error_no << " | " << error << " .. "
            << current_td_pair().location.line << ", "
            << current_td_pair().location.col << std::endl;

  std::cerr << "Current token : " << token_to_str(current_td_pair());
  _parser_okay = false;
}

void parser_c::expect(const token_e token, const std::string &error,
                      const size_t ahead) {
  if (peek(ahead).token != token) {
    die(0, error); // Todo: Update this error code to "unexpected token" code
  }
}

std::vector<node_c *> parser_c::parse_file(const std::string &file) {
  _idx = 0;
  _mark = 0;
  _parser_okay = true;
  _source_name = file;

  _tokens = read_and_lex_file(file);

  if (_tokens.empty()) {
    return {};
  }

  std::vector<node_c *> results;
  while (_parser_okay && _idx < _tokens.size()) {
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

node_c *parser_c::statement() {
  for (auto &&func : _statement_functions) {
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

node_c *parser_c::let_statement() {
  if (current_td_pair().token != token_e::LET) {
    return nullptr;
  }
  auto location = current_td_pair().location;

  advance();

  if (current_td_pair().token != token_e::ID) {
    die(0, "Expected ID");
    return nullptr;
  }
  auto id = current_td_pair().data;
  auto id_loc = current_td_pair().location;

  advance();

  if (current_td_pair().token != token_e::EQ) {
    die(0, "Expected '=' for assignment'");
    return nullptr;
  }

  advance();

  auto exp = expression(precedence_e::LOWEST);
  if (!exp) {
    die(0, "Invalid expression");
    return nullptr;
  }
  advance();

  display_expr_tree("", exp);

  if (current_td_pair().token != token_e::SEMICOLON) {
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

parser_c::precedence_e parser_c::peek_precedence() {
  if (precedences.find(peek().token) != precedences.end()) {
    return precedences[peek().token];
  }
  return parser_c::precedence_e::LOWEST;
}

node_c *parser_c::prefix_expr() {
  std::cout << "prefix" << std::endl;
  node_c *node = nullptr;
  switch (current_td_pair().token) {
  case token_e::SUB:
    node = new node_c(node_type::SUB, current_td_pair().location, "-");
    break;
  default:
    die(0, "Invalid prefix token hit");
    return nullptr;
  }
  advance();
  node->right = expression(precedence_e::PREFIX);
  return node;
}

node_c *parser_c::infix_expr(node_c *left) {

  node_c *node = nullptr;
  switch (current_td_pair().token) {
  case token_e::ADD:
    std::cout << "infix - add" << std::endl;
    node = new node_c(node_type::ADD, current_td_pair().location, "+");
    break;
  case token_e::SUB:
    node = new node_c(node_type::SUB, current_td_pair().location, "-");
    break;
  case token_e::DIV:
    node = new node_c(node_type::ADD, current_td_pair().location, "/");
    break;
  case token_e::MUL:
    node = new node_c(node_type::ADD, current_td_pair().location, "*");
    break;
  case token_e::EQ:
    node = new node_c(node_type::EQ, current_td_pair().location, "=");
    break;
  case token_e::EQ_EQ:
    node = new node_c(node_type::EQ_EQ, current_td_pair().location, "==");
    break;
  case token_e::LT:
    node = new node_c(node_type::LT, current_td_pair().location, "<");
    break;
  case token_e::LTE:
    node = new node_c(node_type::LTE, current_td_pair().location, "<=");
    break;
  case token_e::GT:
    node = new node_c(node_type::GT, current_td_pair().location, ">");
    break;
  case token_e::GTE:
    node = new node_c(node_type::GTE, current_td_pair().location, ">=");
    break;
  case token_e::OR:
    node = new node_c(node_type::OR, current_td_pair().location, "or");
    break;
  case token_e::AND:
    node = new node_c(node_type::AND, current_td_pair().location, "and");
    break;
  case token_e::HAT:
    node = new node_c(node_type::POW, current_td_pair().location, "^");
    break;
  default:
    die(0, "Invalid infix token hit");
    return nullptr;
  }

  precedence_e p = precedence_e::LOWEST;
  if (precedences.find(current_td_pair().token) != precedences.end()) {
    p = precedences[current_td_pair().token];
  }

  advance();
  node->left = left;
  node->right = expression(p);
  return node;
}

node_c *parser_c::grouped_expr() {
  std::cout << "groiuped" << std::endl;
  advance();
  auto expr = expression(precedence_e::LOWEST);

  advance();
  if (current_td_pair().token != token_e::R_PAREN) {
    return nullptr;
  }
  return expr;
}

node_c *parser_c::expression(precedence_e precedence) {

  std::cout << "expr" << std::endl;

  if (_prefix_fns.find(current_td_pair().token) == _prefix_fns.end()) {
    die(0, "no thing for prefix tok");
    return nullptr;
  }

  auto fn = _prefix_fns[current_td_pair().token];
  auto left = (this->*fn)();

  while (peek().token != token_e::SEMICOLON && precedence < peek_precedence()) {
    if (_infix_fns.find(peek().token) == _infix_fns.end()) {
      return left;
    }
    auto i_fn = _infix_fns[peek().token];
    advance();

    left = (this->*i_fn)(left);
  }
  return left;
}

node_c *parser_c::identifier() {
  std::cout << "ident" << std::endl;

  return new node_c(node_type::ID, current_td_pair().location,
                    current_td_pair().data);
}

node_c *parser_c::number() {
  std::cout << "number" << std::endl;
  if (current_td_pair().token == token_e::INTEGER) {
    return new node_c(node_type::INTEGER, current_td_pair().location,
                      current_td_pair().data);
  }
  if (current_td_pair().token == token_e::FLOAT) {
    return new node_c(node_type::FLOAT, current_td_pair().location,
                      current_td_pair().data);
  }
  die(0, "Expected numerical value");
  return nullptr;
}

node_c *parser_c::str() {

  std::cout << "str" << std::endl;
  if (current_td_pair().token == token_e::STRING) {
    return new node_c(node_type::STRING, current_td_pair().location,
                      current_td_pair().data);
  }
  die(0, "Expected string");
  return nullptr;
}

} // namespace compiler