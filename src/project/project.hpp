#ifndef ASTRO_PROJECT_HPP
#define ASTRO_PROJECT_HPP

#include <string>
#include <string_view>
#include <vector>
#include <optional>

namespace project {

//! \brief Package information 
struct package_t {
  std::vector<std::string> authors;
  std::string title;
  std::string description;
  std::string license;
  std::string version;
};

//! \brief Binary information
struct bin_t {
  std::string name;
  std::string path;
  std::vector<std::string> modules;
};

//! \brief Astro project details
struct astro_project_t {
  package_t package;
  bin_t bin;
};

//! \brief Attempt t load a project
//! \param name The path to the project directory
//! \returns std::nullopt if the project was not able to be loaded
extern std::optional<astro_project_t> load_project(std::string_view path);

//! \brief Write the project data
//! \param path The path to the project directory
//! \param project The project information to write out
extern bool write_project(std::string_view path, const astro_project_t project);

}

#endif