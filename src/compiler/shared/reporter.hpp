#ifndef COMPILER_SHARED_REPORTER
#define COMPILER_SHARED_REPORTER

#include "common.hpp"
#include "page.hpp"
#include <string>
#include <unordered_map>

namespace compiler {
namespace shared {

//! \brief Origin of the report
enum class report_origin_e {
  INTERNAL,
  PARSER,
  SEMANTIC_ANALYZER,
};

//! \brief Level, or classification of the report
enum class level_e { ERROR, WARN, INFO };

//! \brief Base report information
class base_report_c {
public:
  base_report_c(const report_origin_e &origin, const level_e &level,
                const std::string &report)
      : origin(origin), level(level), report(report) {}
  report_origin_e origin;
  level_e level;
  std::string report;
};

//! \brief A report about source code that needs to be marked up on output
class marked_source_report_c : public base_report_c {
public:
  //! \brief Source window - How many lines to show above and below the
  //!        reported location
  struct window_t {
    std::size_t top{5};
    std::size_t bottom{5};
  };

  //! \brief Constructor that sets all the data :)
  marked_source_report_c(const report_origin_e &origin, const level_e &level,
                         const std::string &report,
                         const std::string &source_file,
                         const location_c &location,
                         const uint64_t &id)
      : base_report_c(origin, level, report), file(source_file),
        location(location), error_id(id) {}

  std::string file;
  shared::location_c location;
  uint64_t error_id{0};

  window_t window;
};

//! \brief Actual reporting object
class reporter_c {

public:
  //! \brief Create a reporter with a page pool that will be updated externally
  reporter_c(std::unordered_map<std::string, shared::page_c> &page_pool);

  //! \brief Display a marked source report
  //! \returns false iff the source file could not be found
  bool marked_report(const marked_source_report_c &report);
  void standard_report(const base_report_c &report);

private:
  std::unordered_map<std::string, shared::page_c> &_page_pool;
};

} // namespace shared
} // namespace compiler

#endif