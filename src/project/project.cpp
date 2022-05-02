#include "project.hpp"
#include "external/toml.hpp"
#include <filesystem>
#include <iostream>
#include <string_view>

namespace project {

void dump_project_contents(const astro_project_t &project) {
  std::cout << "\n---- Package Information ----"
            << "\nTitle:\t\t" << project.package.title << "\nDescription:\t"
            << project.package.description << "\nLicense:\t"
            << project.package.license << "\nVersion:\t"
            << project.package.version << std::endl;

  if (!project.package.authors.empty()) {
    std::cout << "Authors: " << std::endl;
    for (auto auth : project.package.authors) {
      std::cout << "\t" << auth << std::endl;
    }
  }

  std::cout << "\n---- Binary Information ----"
            << "\nName:\t" << project.bin.name
            << "\nEntry:\t" << project.bin.entry << std::endl;

  if (!project.bin.modules.empty()) {
    std::cout << "Modules: " << std::endl;
    for (auto m : project.bin.modules) {
      std::cout << "\t" << m << std::endl;
    }
  }
}

std::optional<astro_project_t> load_project(std::string_view path) {
  if (!std::filesystem::is_directory(path)) {
    std::cerr << "Given path is not a directory: " << path << std::endl;
    return std::nullopt;
  }

  std::filesystem::path root_path = path;
  root_path /= "astro.toml";

  if (!std::filesystem::is_regular_file(root_path)) {
    std::cerr << "Given directory '" << path
              << "' does not contain the root file '" << root_path << "'"
              << std::endl;
    return std::nullopt;
  }

  toml::table toml_project;
  try {
    toml_project = toml::parse_file(root_path.string());
  } catch (const toml::parse_error &err) {
    std::cerr << "Failed to parse " << root_path << ":\n" << err << std::endl;
    return std::nullopt;
  }

  astro_project_t project;

  // Authors
  auto package_authors = toml_project["package"]["authors"].as_array();
  if (package_authors) {
    for (int i = 0; i < package_authors->size(); i++) {
      auto value = package_authors->get_as<std::string>(i);
      if (value) {
        project.package.authors.push_back(std::string(*value));
      }
    }
  }

  // Description
  std::optional<std::string_view> package_description =
      toml_project["package"]["description"].value_or<std::string_view>("");
  if (package_description == std::nullopt) {
    std::cerr << "Error: package description not given" << std::endl;
    return std::nullopt;
  }
  project.package.description = *package_description;

  // License
  std::optional<std::string_view> package_license =
      toml_project["package"]["license"].value_or<std::string_view>("");
  if (package_license == std::nullopt) {
    std::cerr << "Error: package license not given" << std::endl;
    return std::nullopt;
  }
  project.package.license = *package_license;

  // Title
  std::optional<std::string_view> package_title =
      toml_project["package"]["title"].value<std::string_view>();
  if (package_title == std::nullopt) {
    std::cerr << "Error: package title not given" << std::endl;
    return std::nullopt;
  }
  project.package.title = *package_title;

  // Version
  std::optional<std::string_view> package_version =
      toml_project["package"]["version"].value_or<std::string_view>("");
  if (package_version == std::nullopt) {
    std::cerr << "Error: package version not given" << std::endl;
    return std::nullopt;
  }
  project.package.version = *package_version;

  // Modules
  auto bin_modules = toml_project["bin"]["modules"].as_array();
  if (bin_modules) {
    for (int i = 0; i < bin_modules->size(); i++) {
      auto value = bin_modules->get_as<std::string>(i);
      if (value) {
        project.bin.modules.push_back(std::string(*value));
      }
    }
  }

  // Name
  std::optional<std::string_view> bin_name =
      toml_project["bin"]["name"].value<std::string_view>();
  if (bin_name == std::nullopt) {
    std::cerr << "Error: binary name not given" << std::endl;
    return std::nullopt;
  }
  project.bin.name = *bin_name;

  // Entry
  std::optional<std::string_view> bin_entry =
      toml_project["bin"]["entry"].value<std::string_view>();
  if (bin_name == std::nullopt) {
    std::cerr << "Error: binary entry not given" << std::endl;
    return std::nullopt;
  }
  project.bin.entry = *bin_entry;

  return project;
}

bool write_project(std::string_view path, const astro_project_t project) {
  if (!std::filesystem::is_directory(path)) {
    if (!std::filesystem::create_directories(path)) {
      std::cerr << "Unable to create path : " << path << std::endl;
      return false;
    }
  }

  {
    std::filesystem::path p = project.bin.entry;
    p = p.parent_path();
    if (!std::filesystem::is_directory(p)) {
      if (!std::filesystem::create_directories(p)) {
        std::cerr << "Unable to create path : " << p << std::endl;
        return false;
      }
    }
  }

  std::filesystem::path root_path = path;
  auto project_title = root_path.parent_path().filename();
  if (project_title.empty()) {
    project_title = path;
  }
  root_path /= "astro.toml";

  toml::array authors;
  for (auto item : project.package.authors) {
    authors.push_back(item);
  }

  toml::array modules;
  for (auto item : project.bin.modules) {
    modules.push_back(item);
  }

  auto table = toml::table{

      {"package", toml::table{{"authors", authors},
                              {"title", project.package.title},
                              {"description", project.package.description},
                              {"license", project.package.license},
                              {"version", project.package.version}}},

      {"bin", toml::table{{"name", project.bin.name},
                          {"entry", project.bin.entry},
                          {"modules", modules}}

      }};

  std::fstream out;
  out.open(root_path, std::fstream::out | std::fstream::trunc);
  if (!out.is_open()) {
    std::cerr << "Unable to open " << root_path << " to write project data"
              << std::endl;
    return false;
  }

  out << table;
  return true;
}

} // namespace project