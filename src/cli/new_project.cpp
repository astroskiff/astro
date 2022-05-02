#include "cli.hpp"
#include "project/project.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace cli {

bool new_project(std::string_view name) {
  if (std::filesystem::is_directory(name)) {
    std::cerr << "Directory already exists with name : " << name << std::endl;
    return false;
  }

  std::filesystem::path root_path = name;
  std::filesystem::path src_path = name;

  auto project_title = root_path.parent_path().filename();
  if (project_title.empty()) {
    project_title = name;
  }

  root_path /= "astro.toml";
  src_path /= "src";

  if (!std::filesystem::create_directories(name)) {
    std::cerr << "Unable to create path : " << name << std::endl;
    return false;
  }

  if (!std::filesystem::create_directories(src_path)) {
    std::cerr << "Unable to create path : " << src_path << std::endl;
    return false;
  }

  {
    std::fstream out;
    out.open(root_path.string(), std::fstream::out | std::fstream::trunc);
    if (!out.is_open()) {
      std::cerr << "Unable to create " << root_path << std::endl;
      return false;
    }
    out.close();
  }

  auto src_file = src_path;
  src_file /= "main.ast";
  {
    std::fstream out;
    out.open(src_file.string(), std::fstream::out | std::fstream::trunc);
    if (!out.is_open()) {
      std::cerr << "Unable to create " << src_file << std::endl;
      return false;
    }
    out << "// main.ast";
    out.close();
  }

  project::astro_project_t project;

  project.package.authors = {"unknown"};
  project.package.title = project_title;
  project.package.description = "none";
  project.package.license = "unspecified";
  project.package.version = "0.0.0";

  project.bin.name = project_title;
  project.bin.entry = src_file.string();

  return project::write_project(name, project);
}

} // namespace cli