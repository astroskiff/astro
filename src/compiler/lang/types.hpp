#ifndef COMPILER_LANG_TYPES_HPP
#define COMPILER_LANG_TYPES_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace compiler {
namespace lang {
namespace types {

enum class type_e {
  U8 = 2,
  U16 = 8,
  U32 = 14,
  U64 = 20,
  I8 = 4,
  I16 = 10,
  I32 = 16,
  I64 = 22,
  FLOAT = 50,
  CONTAINER = 100,
  STRUCT = 101
};

enum class container_variant_e { FIXED, VARIABLE };

enum class conversion_e {
  TRUNCATE = 0, // Convert through truncation
  DIRECT,       // Convert directly
  INDIRECT,     // Convert indirectly (container type)
  PROMOTION,    // Promote to type
  INVALID       // Invalid conversion
};

class base_type_c {
public:
  base_type_c(const std::string &name, const type_e type)
      : name(name), type(type) {}
  virtual ~base_type_c() = default;

  std::string name;
  type_e type;
};
using base_type_ptr = std::unique_ptr<base_type_c>;

class container_c : public base_type_c {
public:
  container_c(const std::vector<type_e> &contained_types,
              const container_variant_e &variant)
      : base_type_c("container", type_e::CONTAINER),
        contained_types(contained_types), variant(variant) {}
  std::vector<type_e> contained_types;
  container_variant_e variant;
};

class struct_c : public base_type_c {
public:
  struct_c(const std::unordered_map<std::string, type_e> &member_types)
      : base_type_c("struct", type_e::STRUCT), member_types(member_types) {}

  std::unordered_map<std::string, type_e> member_types;
};

class u8_c : public base_type_c {
public:
  u8_c() : base_type_c("u8", type_e::U8) {}
};

class u16_c : public base_type_c {
public:
  u16_c() : base_type_c("u16", type_e::U16) {}
};

class u32_c : public base_type_c {
public:
  u32_c() : base_type_c("u32", type_e::U32) {}
};

class u64_c : public base_type_c {
public:
  u64_c() : base_type_c("u64", type_e::U64) {}
};

class i8_c : public base_type_c {
public:
  i8_c() : base_type_c("i8", type_e::I8) {}
};

class i16_c : public base_type_c {
public:
  i16_c() : base_type_c("i16", type_e::I16) {}
};

class i32_c : public base_type_c {
public:
  i32_c() : base_type_c("i32", type_e::I32) {}
};

class i64_c : public base_type_c {
public:
  i64_c() : base_type_c("i64", type_e::I64) {}
};

class float_c : public base_type_c {
public:
  float_c() : base_type_c("float", type_e::FLOAT) {}
};

extern conversion_e check_conversion(base_type_c *from, base_type_c *to);

} // namespace types
} // namespace lang
} // namespace compiler

#endif