#include <filesystem>
#include <iostream>

#include "cli/cli.hpp"
#include "compiler/compiler.hpp"
#include "project/project.hpp"

int do_show_usage(const std::string &name) {
  std::cout << "Usage for " << name << std::endl
            << "[-h, --h]                   Show help" << std::endl
            << "new                         Create something new" << std::endl
            << " -sub commands:" << std::endl
            << "\t"
            << "app <name>" << std::endl
            << std::endl
            << "add                         Add something to existing project"
            << std::endl
            << " -sub commands:" << std::endl
            << "\t"
            << "mod <project> <module name>" << std::endl
            << std::endl
            << "build <project name> [args] Build a project" << std::endl
            << " -args:" << std::endl
            << "\t"
            << "\n--release   | -r \tWith debug symbols"
            << "\n--debug     | -d \tWithout debug symbols"
            << "\n--optimized | -o \tEnable optimizations" << std::endl
            << "dump  <project name>        Show project info" << std::endl;
  return 0;
}

int do_new(std::vector<std::string> args) {

  if (args.size() != 2) {
    std::cerr << "Invalid arguments given to command 'new'" << std::endl;
    return 1;
  }

  if (args[0] != "app") {
    std::cerr << "Unknown subcommand : " << args[0] << std::endl;
    return 1;
  }

  if (!cli::new_project(args[1])) {
    std::cerr << "Failed to create project : " << args[1] << std::endl;
    return 1;
  }

  std::cout << "New project '" << args[1] << "' created" << std::endl;
  return 0;
}

int do_add(std::vector<std::string> args) {
  if (args.size() != 3) {
    std::cerr << "Invalid arguments given to command 'add'" << std::endl;
    return 1;
  }

  if (args[0] != "mod") {
    std::cerr << "Unknown subcommand : " << args[0] << std::endl;
    return 1;
  }

  if (!std::filesystem::is_directory(args[1])) {
    std::cerr << "Suspected project name argument '" << args[1]
              << "' does not point to a directory" << std::endl;
    return 1;
  }

  if (!cli::add_mod(args[1], args[2])) {
    return 1;
  }
  return 0;
}

int do_show(std::vector<std::string> args) {

  if (args.size() != 1) {
    std::cerr << "Invalid arguments given to command 'show'" << std::endl;
    return 1;
  }

  if (!std::filesystem::is_directory(args[0])) {
    std::cerr << "Suspected project name argument '" << args[0]
              << "' does not point to a directory" << std::endl;
    return 1;
  }

  auto proj = project::load_project(args[0]);
  if (proj == std::nullopt) {
    std::cerr << "Failed to load project" << std::endl;
    return 1;
  }

  project::dump_project_contents(*proj);
  return 0;
}

int do_build(std::vector<std::string> args) {
  compiler::options_t options;
  std::string project_name = args[0];
  if (args.size() > 1) {
    for (auto &item : std::vector<std::string>(args.begin() + 1, args.end())) {
      if (item == "--debug" || item == "-d") {
        options.build_type = compiler::build_type_e::DEBUG;
        continue;
      }
      if (item == "--release" || item == "-r") {
        options.build_type = compiler::build_type_e::RELEASE;
        continue;
      }
      if (item == "--optimized" || item == "-o") {
        options.enable_optimizations = true;
        continue;
      }
      std::cerr << "Unknown 'build' argument : " << item << std::endl;
      return 1;
    }
  }

  auto loaded_project = project::load_project(project_name);
  if (loaded_project == std::nullopt) {
    std::cerr << "Failed to load project" << std::endl;
    return false;
  }

  auto [okay, result] = compiler::compile_project(
      compiler::targets_e::SKIFF, // Only supported target for some time to come
      options, *loaded_project);

  return 0;
}

int main(int argc, char **argv) {

  std::vector<std::string> args(argv, argv + argc);

  for (size_t i = 1; i < args.size(); i++) {

    if (args[i] == "-h" || args[i] == "--help") {
      return do_show_usage(args[0]);
    }

    if (args[i] == "new") {
      if (i + 1 == args.size()) {
        std::cerr << "Expected args for 'new' command" << std::endl;
        return 1;
      }
      return do_new(std::vector<std::string>(args.begin() + i + 1, args.end()));
    }

    if (args[i] == "add") {
      if (i + 1 == args.size()) {
        std::cerr << "Expected args for 'add' command" << std::endl;
        return 1;
      }
      return do_add(std::vector<std::string>(args.begin() + i + 1, args.end()));
    }

    if (args[i] == "show") {
      if (i + 1 == args.size()) {
        std::cerr << "Expected args for 'show' command" << std::endl;
        return 1;
      }
      return do_show(
          std::vector<std::string>(args.begin() + i + 1, args.end()));
    }

    if (args[i] == "build") {
      if (i + 1 == args.size()) {
        std::cerr << "Expected args for 'build' command" << std::endl;
        return 1;
      }
      return do_build(
          std::vector<std::string>(args.begin() + i + 1, args.end()));
    }
  }

  return 0;
}