#ifndef COMPILER_MIDDLE_ANALYSIS_SCOPED_SYMBOL_TABLE_HPP
#define COMPILER_MIDDLE_ANALYSIS_SCOPED_SYMBOL_TABLE_HPP

#include "compiler/ast.hpp"

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace compiler {
namespace middle {
namespace analysis {

//! \brief A single table entry
class entry_c {
public:
  virtual ~entry_c() = default;

  //! \brief Entry type
  enum class entry_type_e { MEMBER, FUNCTION };

  entry_c(const entry_type_e &type) : type(type) {}

  entry_type_e type;
};

//! \brief A struct'd type, as eventually we will
//!        have user-declared types and will need to tie
//!        everything together
class type_c : public entry_c {
public:
  type_c(const compiler::data_type_e &type)
      : entry_c(entry_c::entry_type_e::MEMBER), type(type) {}
  compiler::data_type_e type;
  // user_decl_t *declaration;
};

//! \brief A simplified view of a function
class function_c : public entry_c {
public:
  struct param_t {
    std::string name;
    type_c type;
  };
  function_c(const std::string &name, const std::vector<param_t> &parameters,
             const compiler::data_type_e &type)
      : entry_c(entry_c::entry_type_e::FUNCTION), name(name),
        parameters(parameters), return_type(type) {}
  std::string name;
  std::vector<param_t> parameters;
  compiler::data_type_e return_type;
};

//! \brief A single arbitrary scope and the things defined in it
class scope_c {
public:
  scope_c(scope_c *parent);
  ~scope_c();

  //! \brief Attempt to find a definition
  //! \param name The name of the item to find
  //! \note If the item doesn't exist within the current scope,
  //!       it will attempt to query its parent scope to find
  //!       the item
  entry_c *find(const std::string &name);

  //! \brief Add something to the table
  //! \param name The name of the item
  //! \param entry The entry itsself
  //! \note Will overwrite any existing item of given name
  void add(const std::string &name, entry_c *entry);

private:
  scope_c *_parent{nullptr};
  std::unordered_map<std::string, entry_c *> _table;
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
  scope_c *get_current_scope();

private:
  std::stack<scope_c *> _scopes;
};

} // namespace analysis
} // namespace middle
} // namespace compiler

#endif