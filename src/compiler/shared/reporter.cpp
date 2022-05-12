#include "reporter.hpp"
#include "compiler/defines.hpp"

#include <iostream>
#include <iomanip>

namespace compiler {
namespace shared {

namespace {
  std::string str_from_origin(const report_origin_e &origin) {
    switch(origin) {
      case report_origin_e::INTERNAL: return "Internal";
      case report_origin_e::PARSER: return "Parser";
      case report_origin_e::SEMANTIC_ANALYZER: return "Analyzer";
    };
    return "Unknown";
  }

  void display_level(const level_e &level) {
    switch(level) {
      case level_e::ERROR: std::cout << TERM_COLOR_BRIGHT_RED << "Error" << TERM_COLOR_END; break;
      case level_e::WARN: std::cout << TERM_COLOR_BRIGHT_YELLOW << "Warning" << TERM_COLOR_END; break;
      case level_e::INFO: std::cout << TERM_COLOR_BRIGHT_CYAN << "Info" << TERM_COLOR_END; break;
    };
  }
}

reporter_c::reporter_c(
    std::unordered_map<std::string, shared::page_c> &page_pool)
    : _page_pool(page_pool) {}


void reporter_c::standard_report(const base_report_c &report)
{
  display_level(report.level);
  std::cout << " <origin:" << str_from_origin(report.origin) << "> :: "<< report.report << std::endl;
}

bool reporter_c::marked_report(const marked_source_report_c &report) {

  if(_page_pool.find(report.file) == _page_pool.end()) {
    standard_report(base_report_c(report_origin_e::INTERNAL, level_e::ERROR, 
    "No source page found for given error report: " + report.file));
    return false;
  }

  auto page = _page_pool[report.file];
  auto num_lines = page.get_page_size();

  if (num_lines <= report.location.line) {
    standard_report(base_report_c(report_origin_e::INTERNAL, level_e::ERROR, 
    "Reported line out of range : " + std::to_string(report.location.line)));
    return false;
  }

  display_level(report.level);
  std::cout << " id: " << report.error_id << " msg: " << report.report << " <origin:" << str_from_origin(report.origin) << ">\n";
  std::cout << "location: " << report.file 
            << "(" << report.location.line + 1
            << ":" << report.location.col
            << ")\n";
  std::cout << std::endl;

  std::vector<std::string> window_top;
  if(report.location.line >= 0) {
    for(auto i = report.location.line - 1; i > 0 && window_top.size() < report.window.top; i--) {
      auto [okay, line] = page.get_line(i);
      if (okay) {
        window_top.push_back(line);
      }
    }
  }

  std::vector<std::string> window_bottom;
  if(report.location.line < num_lines) {
    for(auto i = report.location.line + 1; i < num_lines && window_bottom.size() < report.window.bottom; i++) {
      auto [okay, line] = page.get_line(i);
      if (okay) {
        window_bottom.push_back(line);
      }
    }
  }

  auto [_, target_line] = page.get_line(report.location.line);

  // Build markup
  std::string markup;
  bool begin = false;
  for(auto i = 0; i < target_line.size(); i++) {

    if(!begin) {
      if (!std::isspace(target_line[i])) {
        begin = true;
      }
    }

    if (i == report.location.col) {
        markup += "^";
        break;
    } else if (begin) {
      markup += "~";
    } else {
      // Mimic whitespace chars
      markup += target_line[i];
    }
  }

  // Draw top
  for(auto i = window_top.rbegin(); i != window_top.rend(); i++ ) {
    std::cout << (*i) << std::endl;
  }

  std::cout << target_line << std::endl;
  std::cout << TERM_COLOR_BRIGHT_MAGENTA << markup << TERM_COLOR_END << std::endl;

  for(auto &line : window_bottom) {
    std::cout << line << std::endl;
  }

  return true;
}

} // namespace shared
} // namespace compiler