#include "scoped_symbol_table.hpp"

namespace compiler {
namespace middle {
namespace analysis {

scope_c::scope_c(scope_c * parent) : parent(parent) {}

scope_c::~scope_c() 
{
  for(auto i = functions.begin(); i != functions.end(); ++i) {
    delete (*i).second;
  }
  for(auto i = members.begin(); i != members.end(); ++i) {
    delete (*i).second;
  }
}

function_c * scope_c::find_function(const std::string &name)
{
  if(functions.find(name) == functions.end()) {
    if(parent) {
      return parent->find(name);
    }
  } else {
    return functions[name];
  }
  return nullptr;
}

type_c * scope_c::find_member(const std::string &name)
{
  if(members.find(name) == members.end()) {
    if(parent) {
      return parent->find(name);
    }
  } else {
    return members[name];
  }
  return nullptr;
}

scoped_symbol_table_c::scoped_symbol_table_c()
{
  // Add one for a global scope
  new_scope();
}

scoped_symbol_table_c::~scoped_symbol_table_c()
{
  while(!_scopes.empty()) {
    delete _scopes.top();
    _scopes.pop();
  }
}

scope_c * scoped_symbol_table_c::get_current_scope()
{
  if(_scopes.empty()) {
    return nullptr;
  }
  return _scopes.top();
}

void scoped_symbol_table_c::new_scope() 
{
  scope_c * parent = nullptr;
  if (!_scopes.empty()) {
    parent = _scopes.top();
  }

  _scopes.push(new scope_c(parent));
}

void scoped_symbol_table_c::pop_scope() 
{
  if(_scopes.empty()) {
    return;
  }
  delete _scopes.top();
  _scopes.pop();
}

} // namespace analysis
} // namespace middle
} // namespace compiler