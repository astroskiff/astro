#include <iostream>

#include "cli/cli.hpp"
#include "project/project.hpp"

int main(int argc, char **argv) {

  if (!cli::new_binary("test_project")) {
    std::cout << "Failed to make proj\n";
  }

  auto proj = project::load_project("test_project");

  if (proj == std::nullopt) {
    std::cout << "Failed to load project" << std::endl;
    return 1;
  }

  project::dump_project_contents(*proj);

  return 0;
}