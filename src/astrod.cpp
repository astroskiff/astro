#include <iostream>

#include "cli/cli.hpp"

int main(int argc, char **argv) {

  if (!cli::new_binary("test_project")) {
    std::cout << "Failed to make proj\n";
  }
  return 0;
}