#include "proc.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

void kuchukbaeva::prInput(std::istream& in, OutList& seque)
{
  std::string name = "";
  LIter< std::pair< std::string, InList > > seqTail = seque.beforeBegin();

  while (in >> name) {
    InList seq;

    while (true) {
      int c = in.get();
      while (c == ' ' || c == '\t' || c == '\r') {
        c = in.get();
      }
      if (c == '\n' || c == EOF) {
        break;
      }
      in.unget();

      unsigned long long val = 0;
      in >> val;
      if (in.fail() || val > static_cast< unsigned long long >(std::numeric_limits< int >::max())) {
          throw std::overflow_error("Overflow");
      }
      seq.push_back(static_cast< int >(val));
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

  List< LCIter< int > > iters;
  for (LCIter< std::pair< std::string, InList > > it = seque.cbegin(); it != seque.cend(); ++it) {
    iters.push_back((*it).second.cbegin());
  }

  bool hasMore = true;
  List< int > sums;

  while (hasMore) {
    hasMore = false;
    List< int > row;
    LIter< LCIter< int > > iterNode = iters.begin();
    LCIter< std::pair< std::string, InList > > seqIt = seque.cbegin();

    int currentSum = 0;
    bool isSum = true;

    while (iterNode != iters.end()) {
      if (*iterNode != (*seqIt).second.cend()) {
        const int val = **iterNode;
        row.push_back(val);

        if (val > 0 && std::numeric_limits< int >::max() - currentSum < val) {
          isSum = false;
        } else {
          currentSum += val;
        }

        ++(*iterNode);
        hasMore = true;
      }
      ++iterNode;
      ++seqIt;
    }

    if (hasMore) {
      if (!isSum) {
        err << "Error: Sum overflow\n";
        return 1;
      }
      sums.push_back(currentSum);

      bool isFirst = true;
      for (LCIter< int > it = row.cbegin(); it != row.cend(); ++it) {
        if (!isFirst) out << " ";
        out << *it;
        isFirst = false;
      }
      out << "\n";
    }
  }

  bool isFirst = true;
  for (LCIter< int > it = sums.cbegin(); it != sums.cend(); ++it) {
    if (!isFirst) {
      out << " ";
    }
    out << *it;
    isFirst = false;
  }
  out << "\n";
  return 0;
}
