#ifndef COMPILER_MIDDLE_ANALYSIS_TYPES_HPP
#define COMPILER_MIDDLE_ANALYSIS_TYPES_HPP

namespace compiler {
namespace middle {
namespace analysis {

//! \brief Base forward-facing types that can be declared
enum class base_forward_types_e {
  U8,
  U16,
  U32,
  U64,
  I8,
  I16,
  I32,
  I64,
  INT,
  UINT,
  DOUBLE,
  BOOL,
  STRING,
  USER_DECL
};

} // namespace analysis
} // namespace middle
} // namespace compiler

#endif