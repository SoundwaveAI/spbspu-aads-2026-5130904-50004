#include <iostream>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Error: filename parameter is missing" << "\n";
    return 1;
  }

  kuchukbaeva::Cmd process;

  try
  {
    process.loadFromFile(argv[1]);
    process.processCmd(std::cin, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 2;
  }
  return 0;
}
