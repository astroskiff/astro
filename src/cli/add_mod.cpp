#include "cli.hpp"
#include "project/project.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace cli {

bool add_mod(std::string_view project, std::string_view mod) {

  std::filesystem::path new_module_directory = project;
  new_module_directory /= mod;

  if (std::filesystem::is_directory(new_module_directory)) {
    std::cerr << "Directory with name '" << mod
              << "' already exists within project" << std::endl;
    return false;
  }

  if (!std::filesystem::create_directory(new_module_directory)) {
    std::cerr << "Failed to create new module directory" << std::endl;
    return false;
  }

  new_module_directory /= "mod.ast";

  {
    std::fstream out;
    out.open(new_module_directory.string(),
             std::fstream::out | std::fstream::trunc);
    if (!out.is_open()) {
      std::cerr << "Failed to write module file to new module directory"
                << std::endl;
      new_module_directory = mod;

      // If we failed to create the file then we should remove the directory
      if (!std::filesystem::remove(new_module_directory.string())) {
        std::cerr << "Failed to remove created moduile directory" << std::endl;
      }
      return false;
    }
    out << "// Module: " << mod << "\n";
    out.close();
  }

  project::astro_project_t loaded_project;
  {
    auto proj = project::load_project(project);
    if (proj == std::nullopt) {
      std::cerr << "Failed to load project '" << project << "'" << std::endl;
      return false;
    }
    loaded_project = *proj;
  }

  loaded_project.bin.modules.push_back(std::string(mod));

  if (!project::write_project(project, loaded_project)) {
    std::cerr << "Failed to update project toml" << std::endl;
    return false;
  }

  std::cout << "New module '" << mod << "' was added to project '" << project
            << "'" << std::endl;
  return true;
}

} // namespace cli