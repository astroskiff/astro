#include "reporter.hpp"

namespace compiler {
namespace shared {

reporter_c::reporter_c(
    std::unordered_map<std::string, shared::page_c> &page_pool)
    : _page_pool(page_pool) {}

bool reporter_c::marked_report(const marked_source_report_c &report) {
  return true;
}

void reporter_c::standard_report(const base_report_c &report) {}

} // namespace shared
} // namespace compiler