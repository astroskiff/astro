#ifndef ASTRO_ERROR_MANAGER_HPP
#define ASTRO_ERROR_MANAGER_HPP

#include <string>
#include <unordered_map>

namespace error {

//! \brief Manager for raising and counting errors
class manager_c {
public:
  manager_c();

  //! \brief Raise an error
  //! \param reporter The module that the error came from
  //! \param error_number The number to raise
  //! \param display Optional text to display along with error message
  void raise(std::string_view reporter, uint16_t error_number,
             std::string_view display);

  //! \brief Retrieve the number of errors generated
  uint64_t get_num_errors() const { return _num_errors; }

private:
  uint64_t _num_errors{0};
  std::unordered_map<uint16_t, std::string> _error_map;
};

} // namespace error

#endif