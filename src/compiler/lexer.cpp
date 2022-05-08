#include "lexer.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>

namespace compiler {

lexer_c::lexer_c() : _idx(0) {
  _reserved["id"] = token_e::ID;
  _reserved["for"] = token_e::FOR;
  _reserved["to"] = token_e::TO;
  _reserved["step"] = token_e::STEP;
  _reserved["goto"] = token_e::GOTO;
  _reserved["gosub"] = token_e::GOSUB;
  _reserved["if"] = token_e::IF;
  _reserved["then"] = token_e::THEN;
  _reserved["else"] = token_e::ELSE;
  _reserved["elif"] = token_e::ELIF;
  _reserved["let"] = token_e::LET;
  _reserved["print"] = token_e::PRINT;
  _reserved["read"] = token_e::READ;
  _reserved["return"] = token_e::RETURN;
  _reserved["not"] = token_e::NOT;
  _reserved["and"] = token_e::AND;
  _reserved["or"] = token_e::OR;
}

void lexer_c::clear() {
  _tokens.clear();
  _current_line.clear();
  _idx = 0;
}

std::vector<td_pair_t> lexer_c::lex(size_t line_no, std::string line) {
  _current_line = line;

  for (_idx = 0; _idx < _current_line.size(); advance()) {
    switch (_current_line[_idx]) {
    case '\'': {
      advance();
      std::string label;
      while (std::isalnum(peek()) || std::isdigit(peek()) || peek() == '_') {
        label += _current_line[_idx];
        advance();
      }
      label += _current_line[_idx];
      _tokens.emplace_back(td_pair_t{token_e::LABEL, label, {line_no, _idx}});
      break;
    }

    case ';':
      _tokens.emplace_back(td_pair_t{token_e::SEMICOLON, ";", {line_no, _idx}});
      break;

    case ':':
      _tokens.emplace_back(td_pair_t{token_e::COLON, ":", {line_no, _idx}});
      break;

    case '(':
      _tokens.emplace_back(td_pair_t{token_e::L_PAREN, "(", {line_no, _idx}});
      break;

    case ')':
      _tokens.emplace_back(td_pair_t{token_e::R_PAREN, ")", {line_no, _idx}});
      break;

    case ',':
      _tokens.emplace_back(td_pair_t{token_e::COMMA, ",", {line_no, _idx}});
      break;

    case '#':
      _tokens.emplace_back(td_pair_t{token_e::OCTOTHORP, "#", {line_no, _idx}});
      break;

    case '{':
      _tokens.emplace_back(td_pair_t{token_e::L_BRACE, "{", {line_no, _idx}});
      break;

    case '}':
      _tokens.emplace_back(td_pair_t{token_e::R_BRACE, "}", {line_no, _idx}});
      break;

    case '>':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::GTE, ">=", {line_no, _idx}});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::GT, ">", {line_no, _idx}});
      }
      break;

    case '<':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::LTE, "<=", {line_no, _idx}});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::LT, "<", {line_no, _idx}});
      }
      break;

    case '!':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::NOT_EQ, "!=", {line_no, _idx}});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::NOT, "!", {line_no, _idx}});
      }
      break;

    case '&':
      if (peek() == '&') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::AND, "&&", {line_no, _idx}});
      }
      break;

    case '|':
      if (peek() == '|') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::OR, "||", {line_no, _idx}});
      }
      break;

    case '+':
      _tokens.emplace_back(td_pair_t{token_e::ADD, "+", {line_no, _idx}});
      break;

    case '-':
      _tokens.emplace_back(td_pair_t{token_e::SUB, "-", {line_no, _idx}});
      break;

    case '/':
      _tokens.emplace_back(td_pair_t{token_e::DIV, "/", {line_no, _idx}});
      break;

    case '*':
      _tokens.emplace_back(td_pair_t{token_e::MUL, "*", {line_no, _idx}});
      break;

    case '^':
      _tokens.emplace_back(td_pair_t{token_e::HAT, "^", {line_no, _idx}});
      break;

    case '=':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::EQ_EQ, "==", {line_no, _idx}});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::EQ, "=", {line_no, _idx}});
      }
      break;

    case '"': {
      std::string value;
      bool consume_string = true;
      while (consume_string && peek() != '\0') {
        if ('\\' == _current_line[_idx]) {
          value += _current_line[_idx];
          advance();
          if ('\"' == _current_line[_idx]) {
            value += _current_line[_idx];
            advance();
            continue;
          }
          if ('\n' == _current_line[_idx]) {
            value += static_cast<char>(10);
            advance();
            continue;
          }
        }
        if ('"' == peek()) {
          consume_string = false;
          value += _current_line[_idx];
          advance();
          continue;
        }
        if ('"' != _current_line[_idx]) {
          value += _current_line[_idx];
        }
        advance();
      }

      _tokens.emplace_back(td_pair_t{token_e::STRING, value, {line_no, _idx}});
      break;
    }
    default:
      if (isspace(_current_line[_idx])) {
        break;
      }

      if (std::isdigit(_current_line[_idx])) {
        bool is_float = false;
        std::string item;
        while (std::isdigit(peek()) || (!is_float && peek() == '.')) {
          item += _current_line[_idx];
          if (_current_line[_idx] == '.') {
            is_float = true;
          }
          advance();
        }
        item += _current_line[_idx];

        if (is_float) {
          _tokens.emplace_back(
              td_pair_t{token_e::FLOAT, item, {line_no, _idx}});
        } else {
          _tokens.emplace_back(
              td_pair_t{token_e::INTEGER, item, {line_no, _idx}});
        }
        break;
      }

      // Eat some word, could be a reserved word or an identifier
      std::string word;
      while (std::isalnum(peek()) || std::isdigit(peek()) || peek() == '_') {
        word += _current_line[_idx];
        advance();
      }

      word += _current_line[_idx];
      if (_reserved.find(word) == _reserved.end()) {
        _tokens.emplace_back(td_pair_t{token_e::ID, word, {line_no, _idx}});
      } else {
        _tokens.emplace_back(td_pair_t{_reserved[word], word, {line_no, _idx}});
      }
      break;
    }
  }
  return _tokens;
}

void lexer_c::advance() { _idx++; }

char lexer_c::peek(size_t ahead) {
  if (_current_line.empty()) {
    return '\0';
  }
  if (_idx + ahead >= _current_line.size()) {
    return '\0';
  }
  return _current_line[_idx + ahead];
}

} // namespace compiler