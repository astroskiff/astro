#include "lexer.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>

namespace compiler {
namespace lang {

lexer_c::lexer_c() : _idx(0) {
  _reserved["fn"] = token_e::FN;
  _reserved["while"] = token_e::WHILE;
  _reserved["for"] = token_e::FOR;
  _reserved["if"] = token_e::IF;
  _reserved["else"] = token_e::ELSE;
  _reserved["return"] = token_e::RETURN;
  _reserved["break"] = token_e::BREAK;
  _reserved["let"] = token_e::LET;
  _reserved["use"] = token_e::USE;
  _reserved["import"] = token_e::IMPORT;
  _reserved["struct"] = token_e::STRUCT;
  _reserved["container"] = token_e::CONTAINER;
  _reserved["fixed"] = token_e::C_FIXED;
  _reserved["variable"] = token_e::C_VARIABLE;
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
    case '(':
      _tokens.emplace_back(td_pair_t{token_e::L_PAREN, "(", line_no, _idx});
      break;

    case ')':
      _tokens.emplace_back(td_pair_t{token_e::R_PAREN, ")", line_no, _idx});
      break;

    case '[':
      _tokens.emplace_back(td_pair_t{token_e::L_BRACKET, "[", line_no, _idx});
      break;

    case ']':
      _tokens.emplace_back(td_pair_t{token_e::R_BRACKET, "]", line_no, _idx});
      break;

    case '{':
      _tokens.emplace_back(td_pair_t{token_e::L_BRACE, "{", line_no, _idx});
      break;

    case '}':
      _tokens.emplace_back(td_pair_t{token_e::R_BRACE, "}", line_no, _idx});
      break;

    case ':':
      _tokens.emplace_back(td_pair_t{token_e::COLON, ":", line_no, _idx});
      break;

    case ';':
      _tokens.emplace_back(td_pair_t{token_e::SEMICOLON, ";", line_no, _idx});
      break;

    case ',':
      _tokens.emplace_back(td_pair_t{token_e::COMMA, ",", line_no, _idx});
      break;

    case '>':
      if (peek() == '>') {
        advance();
        if (peek() == '=') {
          advance();
          _tokens.emplace_back(
              td_pair_t{token_e::RSH_EQ, ">>=", line_no, _idx});
        } else {
          _tokens.emplace_back(td_pair_t{token_e::RSH, ">>", line_no, _idx});
        }
      } else if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::GTE, ">=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::GT, ">", line_no, _idx});
      }
      break;

    case '<':
      if (peek() == '<') {
        advance();
        if (peek() == '=') {
          advance();
          _tokens.emplace_back(
              td_pair_t{token_e::LSH_EQ, "<<=", line_no, _idx});
        } else {
          _tokens.emplace_back(td_pair_t{token_e::LSH, "<<", line_no, _idx});
        }
      } else if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::LTE, "<=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::LT, "<", line_no, _idx});
      }
      break;

    case '@':
      _tokens.emplace_back(td_pair_t{token_e::AT, "@", line_no, _idx});
      break;

    case '$':
      _tokens.emplace_back(td_pair_t{token_e::DOLLAR, "$", line_no, _idx});
      break;

    case '\'':
      _tokens.emplace_back(
          td_pair_t{token_e::SINGLE_QUOTE, "'", line_no, _idx});
      break;

    case '?':
      _tokens.emplace_back(
          td_pair_t{token_e::QUESTION_MARK, "?", line_no, _idx});
      break;

    case '.':
      _tokens.emplace_back(td_pair_t{token_e::PERIOD, ".", line_no, _idx});
      break;

    case '#':
      _tokens.emplace_back(td_pair_t{token_e::OCTOTHORPE, "#", line_no, _idx});
      break;

    case '!':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(
            td_pair_t{token_e::EXCLAMATION_EQ, "!=", line_no, _idx});
      } else {
        _tokens.emplace_back(
            td_pair_t{token_e::EXCLAMATION, "!", line_no, _idx});
      }
      break;

    case '-':
      if (peek() == '>') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::ARROW, "->", line_no, _idx});
      } else if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::SUB_EQ, "-=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::SUB, "-", line_no, _idx});
      }
      break;

    case '+':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::ADD_EQ, "+=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::ADD, "+", line_no, _idx});
      }
      break;

    case '/':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::DIV_EQ, "/=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::DIV, "/", line_no, _idx});
      }
      break;

    case '*':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::MUL_EQ, "*=", line_no, _idx});
      } else if (peek() == '*') {
        advance();
        if (peek() == '=') {
          advance();
          _tokens.emplace_back(
              td_pair_t{token_e::POW_EQ, "**=", line_no, _idx});
        } else {
          _tokens.emplace_back(td_pair_t{token_e::POW, "**", line_no, _idx});
        }
      } else {
        _tokens.emplace_back(td_pair_t{token_e::MUL, "*", line_no, _idx});
      }
      break;

    case '%':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::MOD_EQ, "%=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::MOD, "%", line_no, _idx});
      }
      break;

    case '&':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(
            td_pair_t{token_e::AMPERSAND_EQ, "&=", line_no, _idx});
      } else if (peek() == '&') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::AND, "&&", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::AMPERSAND, "&", line_no, _idx});
      }
      break;

    case '|':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::PIPE_EQ, "|=", line_no, _idx});
      } else if (peek() == '|') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::OR, "||", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::PIPE, "|", line_no, _idx});
      }
      break;

    case '~':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::TILDE_EQ, "~=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::TILDE, "~", line_no, _idx});
      }
      break;

    case '^':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::HAT_EQ, "^=", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::HAT, "^", line_no, _idx});
      }
      break;

    case '=':
      if (peek() == '=') {
        advance();
        _tokens.emplace_back(td_pair_t{token_e::EQ_EQ, "==", line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{token_e::EQ, "=", line_no, _idx});
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

      _tokens.emplace_back(td_pair_t{token_e::STRING, value, line_no, _idx});
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
              td_pair_t{token_e::LITERAL_FLOAT, item, line_no, _idx});
        } else {
          _tokens.emplace_back(
              td_pair_t{token_e::LITERAL_NUMBER, item, line_no, _idx});
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
        _tokens.emplace_back(
            td_pair_t{token_e::IDENTIFIER, word, line_no, _idx});
      } else {
        _tokens.emplace_back(td_pair_t{_reserved[word], {}, line_no, _idx});
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

} // namespace lang
} // namespace compiler