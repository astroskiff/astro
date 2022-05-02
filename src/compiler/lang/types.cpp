#include "types.hpp"

#include <iostream>

namespace compiler {
namespace lang {
namespace types {

conversion_e check_conversion(base_type_c *from, base_type_c *to) {
  if (!from || !to) {
    return conversion_e::INVALID;
  }

  //  Majority of cases
  //
  type_e from_type = from->type;
  type_e to_type = to->type;

  if (from_type == type_e::CONTAINER && to_type == type_e::CONTAINER) {
    auto from_container = reinterpret_cast<container_c *>(from);
    auto to_container = reinterpret_cast<container_c *>(to);
    if (from_container->variant != to_container->variant) {
      return conversion_e::INVALID;
    }
    if (from_container->contained_types.size() !=
        to_container->contained_types.size()) {
      return conversion_e::INVALID;
    }
    for (size_t i = 0; i < from_container->contained_types.size(); i++) {
      if (from_container->contained_types[i] == type_e::CONTAINER ||
          to_container->contained_types[i] == type_e::CONTAINER) {
        return conversion_e::INVALID;
      }
    }
    return conversion_e::INDIRECT;
  }

  if (from_type == type_e::CONTAINER) {
    return conversion_e::INVALID;
  }

  if (to_type == type_e::CONTAINER) {
    return conversion_e::INVALID;
  }

  uint8_t from_value = static_cast<uint8_t>(from_type);
  uint8_t to_value = static_cast<uint8_t>(to_type);

  if (from_value == to_value) {
    return conversion_e::DIRECT;
  }

  if (to_value + 2 == from_value || from_value + 2 == to_value) {
    return conversion_e::DIRECT;
  }

  return (to_value > from_value) ? conversion_e::PROMOTION
                                 : conversion_e::TRUNCATE;
}

} // namespace types
} // namespace lang
} // namespace compiler