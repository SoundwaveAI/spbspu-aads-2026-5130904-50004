#ifndef PROC_HPP
#define PROC_HPP

#include "list.hpp"
#include <string>
#include <iosfwd>

namespace kuchukbaeva {

  using InList = List< int >;
  using OutList = List< std::pair< std::string, InList > >;

  void prInput(std::istream& in, OutList& sequences);
  int execLogic(const OutList& seque, std::ostream& out, std::ostream& err);

}

#endif
