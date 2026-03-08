#include "processing.hpp"
#include <iostream>
#include <limits>

void kuchukbaeva::prInput(std::istream& in, OutList& seque)
{
  std::string name = "";
  LIter< std::pair< std::string, InList > > seqTail = seque.beforeBegin();

  while (in >> name) {
    InList seq;
    LIter< int > tail = seq.beforeBegin();

    while (true) {
      int c = in.get();
      while (c == ' ' || c == '\t' || c == '\r') {
        c = in.get();
      }
      if (c == '\n' || c == EOF) {
        break;
      }
      in.unget();

      int val = 0;
      if (in >> val) {
        tail = seq.insertAfter(tail, val);
      } else {
        in.clear();
        break;
      }
    }
    seqTail = seque.insertAfter(seqTail, std::make_pair(name, std::move(seq)));
    if (in.eof()) {
      break;
    }
  }
}

int kuchukbaeva::execLogic(const OutList& seque, std::ostream& out, std::ostream& err)
{
  if (seque.isEmpty()) {
    out << "0\n";
    return 0;
  }

  bool isFirst = true;
  for (LCIter< std::pair< std::string, InList > > it = seque.cbegin(); it != seque.cend(); ++it) {
    if (!isFirst) {
      out << " ";
    }
    out << (*it).first;
    isFirst = false;
  }
  out << "\n";

  List< LCIter< int > > iters;
  LIter< LCIter< int > > itersTail = iters.beforeBegin();
  for (LCIter< std::pair< std::string, InList > > it = seque.cbegin(); it != seque.cend(); ++it) {
    itersTail = iters.insertAfter(itersTail, (*it).second.cbegin());
  }
}
