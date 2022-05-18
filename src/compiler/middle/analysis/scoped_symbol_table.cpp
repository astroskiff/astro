#include "scoped_symbol_table.hpp"

namespace compiler {
namespace middle {
namespace analysis {
scope_c::scope_c(scope_c *parent) : _parent(parent) {}

scope_c::~scope_c() {
  for (auto i = _table.begin(); i != _table.end(); ++i) {
    delete (*i).second;
  }
}

entry_c *scope_c::find(const std::string &name) {
  if (_table.find(name) == _table.end()) {
    if (_parent) {
      return _parent->find(name);
    }
  } else {
    return _table[name];
  }
  return nullptr;
}

void scope_c::add(const std::string &name, entry_c *entry) {
  if (_table.find(name) != _table.end()) {
    delete _table[name];
  }
  _table[name] = entry;
}

scoped_symbol_table_c::scoped_symbol_table_c() {
  // Add one for a global scope
  new_scope();
}

scoped_symbol_table_c::~scoped_symbol_table_c() {
  while (!_scopes.empty()) {
    delete _scopes.top();
    _scopes.pop();
  }
}

scope_c *scoped_symbol_table_c::get_current_scope() {
  if (_scopes.empty()) {
    return nullptr;
  }
  return _scopes.top();
}

void scoped_symbol_table_c::new_scope() {
  scope_c *parent = nullptr;
  if (!_scopes.empty()) {
    parent = _scopes.top();
  }

  _scopes.push(new scope_c(parent));
}

void scoped_symbol_table_c::pop_scope() {
  if (_scopes.empty()) {
    return;
  }
  delete _scopes.top();
  _scopes.pop();
}

} // namespace analysis
} // namespace middle
} // namespace compiler