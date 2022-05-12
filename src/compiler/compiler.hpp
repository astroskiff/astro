#ifndef COMPILER_HEADER_GUARD_HPP
#define COMPILER_HEADER_GUARD_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace compiler {

//! \brief Compiler targets
enum class targets_e {
  SKIFF //! Skiff VM
};

//! \brief Some options to feed into the compiler
struct options_t {
  bool parse_only{false}; // For testing purposes
};

//! \brief Compile an Astro project
//! \param target The target hardware/architecture
//! \param project The loaded project information
//! \returns Tuple of success boolean (true iff no errors are generated)
//!          along-with the resulting binary
extern std::vector<uint8_t>
compile_project(const targets_e target, std::vector<std::string> include_dirs,
                const std::string &file, const options_t &opts);

} // namespace compiler

#endif