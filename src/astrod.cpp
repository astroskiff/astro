#include <filesystem>
#include <iostream>

#include "compiler/compiler.hpp"

int main(int argc, char **argv) {


  if(argc == 2) {

    auto bin = compiler::compile_project(compiler::targets_e::SKIFF, {}, argv[1]);

  }

  return 0;
}