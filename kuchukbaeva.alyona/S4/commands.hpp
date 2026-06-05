#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iosfwd>
#include "bsTree.hpp"

namespace kuchukbaeva
{
  class Cmd
  {
  public:
    using SingleDataset = BSTree< int, std::string >;
    using DatasetStorage = BSTree< std::string, SingleDataset >;
    using FuncType = void (Cmd::*)(std::istream& in, std::ostream& out);

    Cmd();
    void loadFromFile(const std::string& file);
    void processCmd(std::istream& in, std::ostream& out);

  private:
    DatasetStorage datasets_;
    BSTree< std::string, FuncType > commands_;
    void cmdPrint(std::istream& in, std::ostream& out);
    void cmdComplement(std::istream& in, std::ostream& out);
    void cmdIntersect(std::istream& in, std::ostream& out);
    void cmdUnion(std::istream& in, std::ostream& out);
  };
}

#endif
