#ifndef COMPILER_CONTEXT_HPP
#define COMPILER_CONTEXT_HPP

#include "scope.hpp"

namespace compiler {
namespace context {

//! \brief A context of compiler objects
//!        that allows the grouping of artifacts that belong
//!        together. (Like that within a namespace/ module/ etc)
class context_c {
public:
  context_c(const std::string &name);

private:
  std::string _name;
};

} // namespace context
} // namespace compiler

#endif