#include "cli.hpp"

#include <iostream>

namespace cli
{

bool add_mod(std::string_view name)
{
  std::cout << "Asked to add mod : " << name << std::endl;
  return false;
}

}