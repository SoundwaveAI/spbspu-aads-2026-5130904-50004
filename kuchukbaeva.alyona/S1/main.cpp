#include <iostream>

int main()
{
  kuchukbaeva::OutList seque;
  try {
    kuchukbaeva::prInput(std::cin, seque);
    return kuchukbaeva::execLogic(seque, std::cout, std::cerr);
  } catch (const std::exception& e) {
    std::cerr << "Fatal except: " << e.what() << "\n";
    return 2;
  } catch (...) {
    std::cerr << "Fatal error" << "\n";
    return 2;
  }
}

