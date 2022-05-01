#ifndef COMPILER_HEADER_GUARD_HPP
#define COMPILER_HEADER_GUARD_HPP

#include "project/project.hpp"

#include <cstdint>
#include <tuple>
#include <vector>

namespace compiler {

//! \brief Compiler targets
enum class targets_e {
  SKIFF //! Skiff VM
};

//! \brief Build type
enum class build_type_e { DEBUG, RELEASE };

//! \brief Compiler options
struct options_t {
  build_type_e build_type{build_type_e::DEBUG};
  bool enable_optimizations{false};
};

//! \brief Compiler results
struct results_t {
  std::vector<uint8_t> binary; //! Executable binary
};

//! \brief Compile an Astro project
//! \param target The target hardware/architecture
//! \param project The loaded project information
//! \returns Tuple of success boolean (true iff no errors are generated)
//!          along-with the resulting binary
extern std::tuple<bool, results_t>
compile_project(const targets_e target, const options_t options,
                const project::astro_project_t &project);

} // namespace compiler

#endif