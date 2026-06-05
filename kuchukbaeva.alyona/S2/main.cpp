#include <iostream>
#include <fstream>
#include <string>
#include "evaluator.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "Usage: ./lab [filename]\n";
    return 1;
  }

  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Cannot read file " << argv[1] << "\n";
      return 1;
    }
  }

  std::istream& in = file.is_open() ? static_cast< std::istream& >(file) : std::cin;
  kuchukbaeva::Stack< long long > results;
  std::string line;
  try
  {
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      results.push(kuchukbaeva::evaluateExpression(line));
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  bool first = true;
  while (!results.empty())
  {
    if (!first)
    {
      std::cout << " ";
    }
    std::cout << results.top();
    results.pop();
    first = false;
  }
  std::cout << "\n";
  return 0;
}
