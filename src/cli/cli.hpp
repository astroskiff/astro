#ifndef ASTRO_CLI_HPP
#define ASTRO_CLI_HPP

#include <string>
#include <string_view>

namespace cli
{

//! \brief Create a new binary project
//! \param name The name of the new project
//! \returns true iff the binary project could be created
[[nodiscard]] extern bool new_binary(std::string_view name);

//! \brief Create a module within a project
//! \param project The name of the project
//! \param module The name of the new module
//! \returns true iff the module could be created within the project
extern bool add_mod(std::string_view project, std::string_view module);

//! \brief Build project
//! \param name The name of the project
//! \returns true iff the project was built successfully
extern bool build_project(std::string_view name);

/*

  The following are being considered

//! \brief Run project
//! \param name The name of the project
//! \returns true iff the project was ran successfully
extern bool run_project(std::string_view name);

//! \brief Test project
//! \param name The name of the project
//! \returns true iff the project was tested successfully
extern bool test_project(std::string_view name);

//! \brief Execute binary
//! \param name The name of the binary
//! \param arguments Arguments to pass to the binary
//! \returns true iff the binary was executed successfully
extern bool execute_binary(std::string_view name, std::vector<std::string> arguments);

*/
}

#endif 