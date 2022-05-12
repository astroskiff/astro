#include "parser.hpp"
#include "compiler/errors.hpp"
#include "compiler/shared/page.hpp"
#include "compiler/shared/reporter.hpp"
#include "lexer.hpp"
#include "tokens.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <tuple>

namespace compiler {
namespace front {

namespace {

std::tuple<std::vector<td_pair_t>, shared::page_c>
read_and_lex_file(const std::string &file) {

  std::vector<td_pair_t> result;
  shared::page_c file_data;
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
  return {result, file_data};
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
    {token_e::MOD, parser_c::precedence_e::SUM},
    {token_e::DIV, parser_c::precedence_e::PROD},
    {token_e::MUL, parser_c::precedence_e::PROD},
    {token_e::HAT, parser_c::precedence_e::POW},
    {token_e::OR, parser_c::precedence_e::LOGICAL},
    {token_e::AND, parser_c::precedence_e::LOGICAL},
    {token_e::NOT_EQ, parser_c::precedence_e::LOGICAL},
    {token_e::LSH, parser_c::precedence_e::SHIFT},
    {token_e::RSH, parser_c::precedence_e::SHIFT},
    {token_e::BITWISE_AND, parser_c::precedence_e::BITWISE},
    {token_e::BITWISE_NOT, parser_c::precedence_e::BITWISE},
    {token_e::BITWISE_OR, parser_c::precedence_e::BITWISE},
    {token_e::BITWISE_XOR, parser_c::precedence_e::BITWISE},
    {token_e::L_BRACE, parser_c::precedence_e::INDEX},
};

} // namespace

parser_c::parser_c() {
  _statement_functions = {
      std::bind(&parser_c::call_statement, this),
      std::bind(&parser_c::let_statement, this),
      std::bind(&parser_c::label_statement, this),
      std::bind(&parser_c::for_statement, this),
      std::bind(&parser_c::goto_statement, this),
      std::bind(&parser_c::gosub_statement, this),
      std::bind(&parser_c::if_statement, this),
      std::bind(&parser_c::return_statement, this),
      std::bind(&parser_c::asm_statement, this),
      std::bind(&parser_c::reassign_statement, this),
  };

  _prefix_fns[token_e::ID] = &parser_c::identifier;
  _prefix_fns[token_e::INTEGER] = &parser_c::number;
  _prefix_fns[token_e::FLOAT] = &parser_c::number;
  _prefix_fns[token_e::STRING] = &parser_c::str;
  _prefix_fns[token_e::SUB] = &parser_c::prefix_expr;
  _prefix_fns[token_e::L_PAREN] = &parser_c::grouped_expr;
  _prefix_fns[token_e::NOT] = &parser_c::grouped_expr;
  _prefix_fns[token_e::BITWISE_NOT] = &parser_c::grouped_expr;
  _prefix_fns[token_e::L_BRACE] = &parser_c::constructor_list;

  _infix_fns[token_e::NOT_EQ] = &parser_c::infix_expr;
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
  _infix_fns[token_e::NOT_EQ] = &parser_c::infix_expr;
  _infix_fns[token_e::MOD] = &parser_c::infix_expr;
  _infix_fns[token_e::LSH] = &parser_c::infix_expr;
  _infix_fns[token_e::RSH] = &parser_c::infix_expr;
  _infix_fns[token_e::BITWISE_AND] = &parser_c::infix_expr;
  _infix_fns[token_e::BITWISE_OR] = &parser_c::infix_expr;
  _infix_fns[token_e::BITWISE_XOR] = &parser_c::infix_expr;
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

void parser_c::die(uint64_t error_no, std::string error, bool basic_error) {

  compiler::shared::reporter_c reporter(_pages);
  if (basic_error) {
    reporter.standard_report(compiler::shared::base_report_c(
        compiler::shared::report_origin_e::PARSER,
        compiler::shared::level_e::ERROR, error));
  } else {
    reporter.marked_report(compiler::shared::marked_source_report_c(
        compiler::shared::report_origin_e::PARSER,
        compiler::shared::level_e::ERROR, error, _source_name,
        current_td_pair().location, error_no));
  }

  /*
    //  Todo : Send this data to the reporter
    //
    std::cerr << "\nError: " << error_no << " | " << error << " .. "
              << current_td_pair().location.line << ", "
              << current_td_pair().location.col << std::endl;

    std::cerr << "Current token : " << token_to_str(current_td_pair())
              << std::endl;
              */
  _parser_okay = false;
}

std::vector<node_c *> parser_c::parse_file(const std::string &file) {
  _idx = 0;
  _mark = 0;
  _parser_okay = true;
  _source_name = file;

  auto [tokens, page] = read_and_lex_file(file);
  _tokens = tokens;
  _pages[page.get_file()] = page;

  if (_tokens.empty()) {
    return {};
  }

  std::vector<node_c *> results;
  while (_parser_okay && _idx < _tokens.size()) {
    //  Check for top level items
    //

    auto item = function();
    if (!item) {
      die(compiler::errors::parser::ERROR_FAILED_TO_BUILD_FUNCTION,
          "Failed to build function", true);
      return {};
    }

    //  Add top level items
    //
    results.push_back(item);
  }

  _tokens.clear();

  if (!_parser_okay) {
    for (auto item : results) {
      free_nodes(item);
    }
    results.clear();
  }

  return results;
}

node_c *parser_c::let_statement() {
  if (current_td_pair().token != token_e::LET) {
    return nullptr;
  }
  auto location = current_td_pair().location;

  advance();

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ID");
    return nullptr;
  }
  auto id = current_td_pair().data;
  auto id_loc = current_td_pair().location;

  advance();

  std::string variable_type_name{};

  if (current_td_pair().token == token_e::COLON) {
    advance();
    if (current_td_pair().token != token_e::ID) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
          "Expected type name");
      return nullptr;
    }
    variable_type_name = current_td_pair().data;
    advance();
  }

  if (current_td_pair().token != token_e::EQ) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Expected '=' for assignment'");
    return nullptr;
  }

  advance();

  auto exp = expression(precedence_e::LOWEST);
  if (!exp) {
    die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
        "Invalid expression");
    return nullptr;
  }
  advance();

  if (current_td_pair().token != token_e::SEMICOLON) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Missing ';' at end of 'let' statement");
    return nullptr;
  }
  advance();

  auto assignment_node = new node_c(node_type_e::LET, location, "let");
  auto variable_node = new variable_c(id_loc, id);
  variable_node->type_name = variable_type_name;
  append_node(assignment_node, variable_node);
  append_node(assignment_node, exp);
  return assignment_node;
}

node_c *parser_c::label_statement() {
  if (current_td_pair().token != token_e::LABEL) {
    return nullptr;
  }
  auto label_node = new node_c(node_type_e::LABEL, current_td_pair().location,
                               current_td_pair().data);
  advance();
  return label_node;
}

std::vector<node_c *> parser_c::function_parameters() {

  if (current_td_pair().token != token_e::L_PAREN) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected '('");
    return {};
  }

  advance();

  // Could be empty
  if (current_td_pair().token == token_e::R_PAREN) {
    advance();
    return {};
  }

  // Could have params (param:type, param:type)
  std::vector<node_c *> results;
  while (_parser_okay) {
    if (current_td_pair().token != token_e::ID) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
          "Expected parameter identifier");
      break;
    }
    auto param_name = current_td_pair().data;
    auto param_loc = current_td_pair().location;
    advance();

    if (current_td_pair().token != token_e::COLON) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ':'");
      break;
    }
    advance();

    if (current_td_pair().token != token_e::ID) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
          "Expected parameter type identifier");
      break;
    }
    auto type_name = current_td_pair().data;
    advance();

    results.push_back(new variable_c(param_loc, param_name, type_name));

    // If the next one is an R_PAREN we are done
    if (current_td_pair().token == token_e::R_PAREN) {
      break;
    }

    // If its a comma we get to go around again
    if (current_td_pair().token != token_e::COMMA) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ','");
      break;
    }
    advance();
  }
  advance();
  return results;
}

std::vector<node_c *> parser_c::statement_block() {
  if (current_td_pair().token != token_e::L_BRACE) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected '{'");
    return {};
  }
  advance();

  std::vector<node_c *> results;

  while (_parser_okay) {
    if (current_td_pair().token == token_e::R_BRACE || !_parser_okay) {
      break;
    }
    for (auto &&func : _statement_functions) {
      if (auto result = func(); result) {
        results.push_back(result);
      }
    }
  }

  if (!_parser_okay) {
    for (auto s : results) {
      free_nodes(s);
    }
    return {};
  }

  if (current_td_pair().token != token_e::R_BRACE) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected '}'");
    return {};
  }
  advance();

  return results;
};

std::vector<node_c *> parser_c::call_parameters() {

  if (current_td_pair().token != token_e::L_PAREN) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected '('");
    return {};
  }

  advance();

  // Could be empty
  if (current_td_pair().token == token_e::R_PAREN) {
    return {};
  }

  // Could have params (expression, expression)
  std::vector<node_c *> results;
  while (_parser_okay) {

    auto expr = expression(precedence_e::LOWEST);
    if (!expr) {
      die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
          "Malformed expression ");
      return {};
    }

    results.push_back(expr);
    advance();

    // If the next one is an R_PAREN we are done
    if (current_td_pair().token == token_e::R_PAREN) {
      break;
    }

    // If its a comma we get to go around again
    if (current_td_pair().token != token_e::COMMA) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ','");
      break;
    }
    advance();
  }
  return results;
}

node_c *parser_c::for_statement() {
  if (current_td_pair().token != token_e::FOR) {
    return nullptr;
  }
  auto for_location = current_td_pair().location;

  advance();

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ID");
    return nullptr;
  }
  auto id_location = current_td_pair().location;
  auto id_value = current_td_pair().data;

  advance();

  if (current_td_pair().token != token_e::EQ) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Expected ID assignment ");
    return nullptr;
  }

  advance();

  auto from_expr = expression(precedence_e::LOWEST);
  if (!from_expr) {
    die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
        "Malformed expression ");
    return nullptr;
  }

  advance();

  if (current_td_pair().token != token_e::TO) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected TO");
    return nullptr;
  }

  advance();

  auto to_expr = expression(precedence_e::LOWEST);
  if (!to_expr) {
    die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
        "Malformed expression ");
    return nullptr;
  }

  advance();

  node_c *step_variable{nullptr};

  // Optional STEP
  if (current_td_pair().token == token_e::STEP) {
    advance();
    if (current_td_pair().token != token_e::INTEGER) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
          "Expected integer for step (convert to expression later)");
      return nullptr;
    }
    step_variable = new node_c(node_type_e::INTEGER, current_td_pair().location,
                               current_td_pair().data);
    advance();
  }

  // Loop body
  auto statements = statement_block();
  if (!_parser_okay) {
    for (auto item : statements) {
      free_nodes(item);
    }
    free_nodes(step_variable);
  }

  auto loop = new for_loop_c(for_location);
  loop->from = from_expr;
  loop->to = to_expr;
  loop->step = step_variable;
  loop->body = statements;
  loop->data = "for";
  return loop;
};

node_c *parser_c::goto_statement() {
  if (current_td_pair().token != token_e::GOTO) {
    return nullptr;
  }
  auto goto_location = current_td_pair().location;

  advance();

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected label ID");
    return nullptr;
  }
  auto id_location = current_td_pair().location;
  auto id_value = current_td_pair().data;

  advance();

  if (current_td_pair().token != token_e::SEMICOLON) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ;");
    return nullptr;
  }

  advance();

  auto label_node = new node_c(node_type_e::LABEL, id_location, id_value);
  auto goto_node = new node_c(node_type_e::GOTO, goto_location, "goto");
  append_node(goto_node, label_node);

  return goto_node;
};

node_c *parser_c::gosub_statement() {
  if (current_td_pair().token != token_e::GOSUB) {
    return nullptr;
  }
  auto gosub_location = current_td_pair().location;

  advance();

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected label ID");
    return nullptr;
  }
  auto id_location = current_td_pair().location;
  auto id_value = current_td_pair().data;

  advance();

  if (current_td_pair().token != token_e::SEMICOLON) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ;");
    return nullptr;
  }

  advance();

  auto label_node = new node_c(node_type_e::LABEL, id_location, id_value);
  auto gosub_node = new node_c(node_type_e::GOTO, gosub_location, "gosub");
  append_node(gosub_node, label_node);

  return gosub_node;
};

node_c *parser_c::asm_statement() {

  if (current_td_pair().token != token_e::ASM) {
    return nullptr;
  }
  auto location = current_td_pair().location;
  advance();

  if (current_td_pair().token != token_e::L_BRACE) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected '{'");
    return {};
  }

  std::vector<node_c *> body;

  while (_parser_okay) {
    advance();

    if (current_td_pair().token == token_e::R_BRACE || !_parser_okay) {
      break;
    }

    if (current_td_pair().token != token_e::STRING) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
          "Expected string in ASM block");
      return nullptr;
    }

    body.push_back(new node_c(node_type_e::STRING, current_td_pair().location,
                              current_td_pair().data));
  }

  if (!_parser_okay) {
    return nullptr;
  }

  if (current_td_pair().token != token_e::R_BRACE) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected '}'");
    return {};
  }
  advance();

  auto asm_node = new bodied_node_c(node_type_e::ASM, location);
  asm_node->data = "asm";
  asm_node->body = body;
  return asm_node;
}

node_c *parser_c::function() {
  if (current_td_pair().token != token_e::FN) {
    return nullptr;
  }
  auto fn_loc = current_td_pair().location;
  advance();

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Expected function name");
    return nullptr;
  }

  auto func_name = current_td_pair().data;
  auto func_name_loc = current_td_pair().location;
  advance();

  auto params = function_parameters();

  if (!_parser_okay) {
    for (auto i : params) {
      free_nodes(i);
    }
    return nullptr;
  }

  if (current_td_pair().token != token_e::ARROW) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Expected '->' to indicate return type");
    for (auto i : params) {
      free_nodes(i);
    }
    return nullptr;
  }
  advance();

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Expected '->' to indicate return type");
    for (auto i : params) {
      free_nodes(i);
    }
    return nullptr;
  }
  auto return_type_name = current_td_pair().data;
  auto return_type_loc = current_td_pair().location;
  advance();

  auto statements = statement_block();
  if (!_parser_okay) {
    for (auto i : params) {
      free_nodes(i);
    }
    for (auto i : statements) {
      free_nodes(i);
    }
    return nullptr;
  }

  return new function_node_c(func_name_loc, func_name, statements, params,
                             return_type_name, return_type_loc);
}

node_c *parser_c::call_statement() {
  if (current_td_pair().token != token_e::ID) {
    return nullptr;
  }
  auto function_name = current_td_pair().data;
  auto location = current_td_pair().location;
  advance();

  auto params = call_parameters();
  if (!_parser_okay) {
    for (auto item : params) {
      free_nodes(item);
    }
    return nullptr;
  }

  advance();
  if (current_td_pair().token != token_e::SEMICOLON) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ';'");
    for (auto item : params) {
      free_nodes(item);
    }
    return nullptr;
  }

  advance();
  return new bodied_node_c(node_type_e::CALL, location, function_name, params);
}

node_c *parser_c::if_statement() {
  if (current_td_pair().token != token_e::IF) {
    return nullptr;
  }
  auto if_location = current_td_pair().location;
  advance();

  auto first_condition = expression(precedence_e::LOWEST);
  if (!first_condition) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
        "Malformed expression ");
    return nullptr;
  }
  advance();

  auto statements = statement_block();
  auto elif = elif_statement();

  auto conditional =
      new bodied_node_c(node_type_e::CONDITIONAL, if_location, statements);
  conditional->data = "if";
  conditional->body = statements;
  append_node(conditional, first_condition);
  if (elif) {
    append_node(conditional, elif);
  }

  auto else_ = else_statement();
  if (else_) {
    append_node(conditional, else_);
  }

  return conditional;
};

node_c *parser_c::elif_statement() {
  if (current_td_pair().token != token_e::ELIF) {
    return nullptr;
  }

  auto location = current_td_pair().location;
  advance();

  auto first_condition = expression(precedence_e::LOWEST);
  if (!first_condition) {
    die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
        "Malformed expression ");
    return nullptr;
  }
  advance();

  auto statements = statement_block();
  auto elif = elif_statement();

  auto conditional =
      new bodied_node_c(node_type_e::CONDITIONAL, location, statements);
  conditional->data = "elif";
  conditional->body = statements;

  append_node(conditional, first_condition);
  if (elif) {
    append_node(conditional, elif);
  }
  return conditional;
}

node_c *parser_c::else_statement() {
  if (current_td_pair().token != token_e::ELSE) {
    return nullptr;
  }

  auto location = current_td_pair().location;
  advance();

  auto statements = statement_block();

  auto conditional =
      new bodied_node_c(node_type_e::CONDITIONAL, location, statements);
  conditional->data = "else";
  conditional->body = statements;
  append_node(conditional, new node_c(node_type_e::INTEGER, location, "1"));
  return conditional;
}

node_c *parser_c::return_statement() {
  if (current_td_pair().token != token_e::RETURN) {
    return nullptr;
  }

  auto return_location = current_td_pair().location;

  advance();
  node_c *return_expression{nullptr};
  if (current_td_pair().token != token_e::SEMICOLON) {

    return_expression = expression(precedence_e::LOWEST);
    if (!return_expression) {
      die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
          "Invalid return expression");
      return nullptr;
    }
    advance();

    if (current_td_pair().token != token_e::SEMICOLON) {
      die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ;");
      free_nodes(return_expression);
      return nullptr;
    }
  }
  advance();

  auto return_node = new node_c(node_type_e::RETURN, return_location);
  return_node->data = "return";
  append_node(return_node, return_expression);
  return return_node;
};

node_c *parser_c::reassign_statement() {
  if (!(current_td_pair().token == token_e::ID &&
        peek().token == token_e::EQ)) {
    return nullptr;
  }

  auto location = current_td_pair().location;
  auto id = current_td_pair().data;

  advance(); // Past id
  advance(); // Past eq

  auto exp = expression(precedence_e::LOWEST);
  if (!exp) {
    die(compiler::errors::parser::ERROR_INVALID_EXPRESSION,
        "Invalid expression");
    return nullptr;
  }
  advance();

  if (current_td_pair().token != token_e::SEMICOLON) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Missing ';'");
    return nullptr;
  }
  advance();

  auto node = new node_c(node_type_e::REASSIGN, location);
  node->data = "reassign";
  auto var = new variable_c(location, id);
  append_node(node, var);
  append_node(node, exp);
  return node;
}

// // // // // // // // // // // // // // // // // // // // // //
//                                                             //
//                Expression parsing logic                     //
//                                                             //
// // // // // // // // // // // // // // // // // // // // // //

parser_c::precedence_e parser_c::peek_precedence() {
  if (precedences.find(peek().token) != precedences.end()) {
    return precedences[peek().token];
  }
  return parser_c::precedence_e::LOWEST;
}

node_c *parser_c::prefix_expr() {
  node_c *node = nullptr;
  switch (current_td_pair().token) {
  case token_e::SUB:
    node = new node_c(node_type_e::SUB, current_td_pair().location, "-");
    break;
  case token_e::NOT:
    node = new node_c(node_type_e::NOT, current_td_pair().location, "!");
    break;
  case token_e::BITWISE_NOT:
    node = new node_c(node_type_e::NOT, current_td_pair().location, "not");
    break;
  default:
    die(compiler::errors::parser::ERROR_UNEXPECTED_PREFIX_TOKEN,
        "Invalid prefix token hit");
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
    node = new node_c(node_type_e::ADD, current_td_pair().location, "+");
    break;
  case token_e::SUB:
    node = new node_c(node_type_e::SUB, current_td_pair().location, "-");
    break;
  case token_e::DIV:
    node = new node_c(node_type_e::ADD, current_td_pair().location, "/");
    break;
  case token_e::MUL:
    node = new node_c(node_type_e::ADD, current_td_pair().location, "*");
    break;
  case token_e::EQ:
    node = new node_c(node_type_e::EQ, current_td_pair().location, "=");
    break;
  case token_e::EQ_EQ:
    node = new node_c(node_type_e::EQ_EQ, current_td_pair().location, "==");
    break;
  case token_e::LT:
    node = new node_c(node_type_e::LT, current_td_pair().location, "<");
    break;
  case token_e::LTE:
    node = new node_c(node_type_e::LTE, current_td_pair().location, "<=");
    break;
  case token_e::GT:
    node = new node_c(node_type_e::GT, current_td_pair().location, ">");
    break;
  case token_e::GTE:
    node = new node_c(node_type_e::GTE, current_td_pair().location, ">=");
    break;
  case token_e::OR:
    node = new node_c(node_type_e::OR, current_td_pair().location, "or");
    break;
  case token_e::AND:
    node = new node_c(node_type_e::AND, current_td_pair().location, "and");
    break;
  case token_e::HAT:
    node = new node_c(node_type_e::POW, current_td_pair().location, "^");
    break;
  case token_e::NOT_EQ:
    node = new node_c(node_type_e::NOT_EQ, current_td_pair().location, "!=");
    break;
  case token_e::LSH:
    node = new node_c(node_type_e::LSH, current_td_pair().location, "lsh");
    break;
  case token_e::RSH:
    node = new node_c(node_type_e::RSH, current_td_pair().location, "rsh");
    break;
  case token_e::MOD:
    node = new node_c(node_type_e::MOD, current_td_pair().location, "mod");
    break;
  case token_e::BITWISE_AND:
    node = new node_c(node_type_e::BW_AND, current_td_pair().location, "and");
    break;
  case token_e::BITWISE_OR:
    node = new node_c(node_type_e::BW_OR, current_td_pair().location, "or");
    break;
  case token_e::BITWISE_XOR:
    node = new node_c(node_type_e::BW_XOR, current_td_pair().location, "xor");
    break;
  default:
    die(compiler::errors::parser::ERROR_UNEXPECTED_INFIX_TOKEN,
        "Invalid infix token hit");
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
  advance();
  auto expr = expression(precedence_e::LOWEST);

  advance();
  if (current_td_pair().token != token_e::R_PAREN) {
    return nullptr;
  }
  return expr;
}

node_c *parser_c::constructor_list() {

  std::cerr << "Constructor list not yest supported" << std::endl;
  die(compiler::errors::parser::ERROR_NOT_YET_SUPPORTED, "Not yet supported");
  return nullptr;
}

node_c *parser_c::expression(precedence_e precedence) {
  if (_prefix_fns.find(current_td_pair().token) == _prefix_fns.end()) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_PREFIX_TOKEN,
        "No matching function for given prefix");
    return nullptr;
  }

  auto fn = _prefix_fns[current_td_pair().token];
  auto left = (this->*fn)();

  while (_parser_okay && peek().token != token_e::SEMICOLON &&
         precedence < peek_precedence()) {
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

  if (current_td_pair().token != token_e::ID) {
    die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected ID");
    return nullptr;
  }

  //  If the next token is a '(' we know that the identifier is meant to
  //  be a function name so lets eat that up
  //
  if (peek().token == token_e::L_PAREN) {
    return call();
  }

  // If the next item is a '[' we know that the identifier is
  // being treated as an array
  //
  if (peek().token == token_e::R_BRACKET) {
    return array_index();
  }

  return new variable_c(current_td_pair().location, current_td_pair().data);
}

node_c *parser_c::call() {
  // Only called by a place that identifies the current item as an ID
  auto function_name = current_td_pair().data;
  auto location = current_td_pair().location;
  advance();

  auto params = call_parameters();
  if (!_parser_okay) {
    for (auto item : params) {
      free_nodes(item);
      return nullptr;
    }
  }

  return new bodied_node_c(node_type_e::CALL, location, function_name, params);
}

node_c *parser_c::array_index() {
  die(compiler::errors::parser::ERROR_NOT_YET_SUPPORTED,
      "Indexing arrays within expressions not yet supported");
  return nullptr;
}

node_c *parser_c::number() {
  if (current_td_pair().token == token_e::INTEGER) {
    return new node_c(node_type_e::INTEGER, current_td_pair().location,
                      current_td_pair().data);
  }
  if (current_td_pair().token == token_e::FLOAT) {
    return new node_c(node_type_e::FLOAT, current_td_pair().location,
                      current_td_pair().data);
  }
  die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN,
      "Expected numerical value");
  return nullptr;
}

node_c *parser_c::str() {
  if (current_td_pair().token == token_e::STRING) {
    return new node_c(node_type_e::STRING, current_td_pair().location,
                      current_td_pair().data);
  }
  die(compiler::errors::parser::ERROR_UNEXPECTED_TOKEN, "Expected string");
  return nullptr;
}

} // namespace front
} // namespace compiler