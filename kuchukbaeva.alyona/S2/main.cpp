#include <iostream>
#include <fstream>
#include <string>
#include "evaluator.hpp"
#include "stack.hpp"

int main(int argc, char* argv[]) {
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Cannot read file " << argv[1] << "\n";
      return 1;
    }
    input = &file;
  } else if (argc > 2) {
    std::cerr << "it must be ./lab {filename}" << "\n";
    return 1;
  }

  kuchukbaeva::Stack< long long > results;
  std::string line = "";
  try {
    while (std::getline(*input, line)) {
      if (line.empty()) {
        continue;
      }
      results.push(kuchukbaeva::evaluateExpression(line));
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  bool first = true;
  while (!results.isEmpty()) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << results.drop();
    first = false;
  }
  std::cout << "\n";
  return 0;
}
