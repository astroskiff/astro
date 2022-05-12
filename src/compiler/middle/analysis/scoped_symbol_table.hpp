#ifndef COMPILER_MIDDLE_ANALYSIS_SCOPED_SYMBOL_TABLE_HPP
#define COMPILER_MIDDLE_ANALYSIS_SCOPED_SYMBOL_TABLE_HPP

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

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

//! \brief A struct'd type, as eventually we will
//!        have user-declared types and will need to tie
//!        everything together
class type_c {
public:
  type_c(const base_forward_types_e &type) : type(type) {}
  base_forward_types_e type;
  // user_decl_t *declaration;
};

//! \brief A simplified view of a function
class function_c {
public:
  struct param_t {
    std::string name;
    type_c type;
  };
  function_c(const std::string &name, const std::vector<param_t> &parameters)
      : name(name), parameters(parameters) {}
  std::string name;
  std::vector<param_t> parameters;
};

//! \brief A single arbitrary scope and the things defined in it
class scope_c {
public:
  scope_c(scope_c * parent);
  ~scope_c();

  scope_c parent{nullptr};
  std::unordered_map<std::string, function_c *> functions;
  std::unordered_map<std::string, type_c *> members;

  //! \brief Attempt to find a definition
  //! \param name The name of the item to find
  //! \note If the item doesn't exist within the current scope,
  //!       it will attempt to query its parent scope to find 
  //!       the item
  function_c * find_function(const std::string &name);

  //! \brief Attempt to find a definition
  //! \param name The name of the item to find
  //! \note If the item doesn't exist within the current scope,
  //!       it will attempt to query its parent scope to find 
  //!       the item
  type_c * find_member(const std::string &name);
};

//! \brief Scope symbol table
class scoped_symbol_table_c {
public:

  scoped_symbol_table_c();
  ~scoped_symbol_table_c();

  //! \brief Create a new scope
  void new_scope();

  //! \brief Pop the current scope
  void pop_scope();

  //! \brief Retrieve the current scope
  //! \returns nullptr iff there is no active scope
  scope_c * get_current_scope();

private:

  std::stack<scope_c*> _scopes;
};

} // namespace analysis
} // namespace middle
} // namespace compiler

#endif