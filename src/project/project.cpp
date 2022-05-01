#include "project.hpp"
#include "external/toml.hpp"
#include <filesystem>
#include <iostream>

namespace project {



std::optional<astro_project_t> load_project(std::string_view path)
{
  std::cout << "Asked to load porject : " << path << std::endl;
  return std::nullopt;
}

bool write_project(std::string_view path, const astro_project_t project)
{
  std::cout << "Asked to write porject : " << path << std::endl;

  if (!std::filesystem::is_directory(path)) {
    if (!std::filesystem::create_directories(path)) {
      std::cerr << "Unable to create path : " << path << std::endl;
      return false;
    }
  }

  if (!std::filesystem::is_directory(project.bin.path)) {
    if (!std::filesystem::create_directories(project.bin.path)) {
      std::cerr << "Unable to create path : " << project.bin.path << std::endl;
      return false;
    }
  }

  std::filesystem::path root_path = path;
  auto project_title = root_path.parent_path().filename();
  if (project_title.empty()) {
    project_title = path;
  }
  root_path /= "astro.toml";

  toml::array authors;
  for(auto item : project.package.authors) {
    authors.push_back(item);
  }

  toml::array modules;
  for(auto item: project.bin.modules) {
    modules.push_back(item);
  }

  auto table = toml::table {

    {"package", toml::table{
      {"authors", authors},
      {"title", project.package.title},
      {"description", project.package.description},
      {"license", project.package.license},
      {"version", project.package.version}
      }
    },

    {"bin", toml::table{
      { "name", project.bin.name },
      { "path", project.bin.path },
      { "modules", modules}
    }
    
    }
  };
  
  std::fstream out;
  out.open(root_path, std::fstream::out | std::fstream::trunc);
  if (!out.is_open()) {
    std::cerr << "Unable to open " << root_path << " to write project data" << std::endl;
    return false;
  }

  out << table;
  return true;
}

}