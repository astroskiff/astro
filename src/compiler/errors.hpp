#ifndef COMPILER_ERRORS_HPP
#define COMPILER_ERRORS_HPP

namespace compiler {
namespace errors {
namespace parser {
constexpr uint64_t ERROR_UNEXPECTED_TOKEN = 100;
constexpr uint64_t ERROR_INVALID_EXPRESSION = 101;
constexpr uint64_t ERROR_FAILED_TO_BUILD_FUNCTION = 101;
constexpr uint64_t ERROR_UNEXPECTED_PREFIX_TOKEN = 102;
constexpr uint64_t ERROR_UNEXPECTED_INFIX_TOKEN = 103;
constexpr uint64_t ERROR_NOT_YET_SUPPORTED = 104;
} // namespace parser
} // namespace errors
} // namespace compiler

#endif