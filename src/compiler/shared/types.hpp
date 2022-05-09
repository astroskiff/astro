#ifndef COMPILER_TYPES_HPP
#define COMPILER_TYPES_HPP

#include <string>
#include <unordered_map>

namespace compiler
{

// FWD Declaration for things in compiler
class node_c;

namespace shared
{

enum class base_type_e {
  INT = 0,
  DOUBLE,
  BOOL,
  STRING,
  OBJECT = 100
};

//! \brief A type representation class
class type_c
{
public:
  type_c(const base_type_e base_type, const std::string &name) : base_type(base_type), name(name){}
  virtual ~type_c() = default;
  std::string name;
  base_type_e base_type;
  node_c * instantiation_expression{nullptr};

  bool instantiation_is_valid();
};





}
}


#endif