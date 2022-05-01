#include "error_manager.hpp"
#include "errors.hpp"

#include <iostream>

namespace error {

manager_c::manager_c() {
  _error_map[error::cli::INVALID_COMMAND] = "Invalid command given";
}

void manager_c::raise(std::string_view reporter, uint16_t error_number,
                      std::string_view display) {
  _num_errors++;
  std::cout << "Error #" << error_number << ": " << _error_map[error_number]
            << "\n"
            << display << std::endl;
}

} // namespace error