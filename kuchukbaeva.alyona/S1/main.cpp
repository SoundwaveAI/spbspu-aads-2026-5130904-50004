#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
#include <utility>
#include <list.hpp>

int main()
{
  try
  {
    kuchukbaeva::List< std::pair< std::string, kuchukbaeva::List< unsigned long long > > > seque;
    kuchukbaeva::LIter< std::pair< std::string,
      kuchukbaeva::List< unsigned long long > > > seqTail = seque.beforeBegin();
    std::string name;

    while (std::cin >> name)
    {
      kuchukbaeva::List< unsigned long long > seq;
      kuchukbaeva::LIter< unsigned long long > tail = seq.beforeBegin();
      unsigned long long val = 0;

      while (std::cin >> val)
      {
        tail = seq.insertAfter(tail, val);
      }

      if (!std::cin.eof())
      {
        std::cin.clear();
      }

      seqTail = seque.insertAfter(seqTail, std::make_pair(name, std::move(seq)));
    }

    if (seque.isEmpty())
    {
      std::cout << "0" << "\n";
      return 0;
    }

    kuchukbaeva::LCIter< std::pair< std::string, kuchukbaeva::List< unsigned long long > > > itName = seque.cbegin();
    if (itName != seque.cend())
    {
      std::cout << itName->first;
      ++itName;
      for (; itName != seque.cend(); ++itName)
      {
        std::cout << " " << itName->first;
      }
      std::cout << "\n";
    }

    kuchukbaeva::List< kuchukbaeva::LCIter< unsigned long long > > iters;
    kuchukbaeva::LIter< kuchukbaeva::LCIter< unsigned long long > > itersTail = iters.beforeBegin();
    for (kuchukbaeva::LCIter< std::pair< std::string,
      kuchukbaeva::List< unsigned long long > > > it = seque.cbegin(); it != seque.cend(); ++it)
    {
      itersTail = iters.insertAfter(itersTail, it->second.cbegin());
    }

    bool hasMore = true;
    kuchukbaeva::List< unsigned long long > sums;
    kuchukbaeva::LIter< unsigned long long > sumsTail = sums.beforeBegin();

    while (hasMore)
    {
      hasMore = false;
      kuchukbaeva::List< unsigned long long > row;
      kuchukbaeva::LIter< unsigned long long > rowTail = row.beforeBegin();
      kuchukbaeva::LIter< kuchukbaeva::LCIter< unsigned long long > > iterNode = iters.begin();
      kuchukbaeva::LCIter< std::pair< std::string, kuchukbaeva::List< unsigned long long > > > seqIt = seque.cbegin();

      unsigned long long currentSum = 0;
      bool isSum = true;
      bool rowHasElements = false;

      while (iterNode != iters.end())
      {
        if (*iterNode != seqIt->second.cend())
        {
          const unsigned long long val = **iterNode;
          rowTail = row.insertAfter(rowTail, val);
          rowHasElements = true;

          if (std::numeric_limits< unsigned long long >::max() - currentSum < val)
          {
            isSum = false;
          }
          else
          {
            currentSum += val;
          }
          ++(*iterNode);
          hasMore = true;
        }
        ++iterNode;
        ++seqIt;
      }
      if (rowHasElements)
      {
        kuchukbaeva::LCIter< unsigned long long > itRow = row.cbegin();
        if (itRow != row.cend())
        {
          std::cout << *itRow;
          ++itRow;
          for (; itRow != row.cend(); ++itRow)
          {
            std::cout << " " << *itRow;
          }
          std::cout << "\n";
        }

        if (!isSum)
        {
          std::cerr << "Error: Overflow\n";
          return 1;
        }
        sumsTail = sums.insertAfter(sumsTail, currentSum);
      }
    }
    if (sums.isEmpty())
    {
      std::cout << "0\n";
    }
    else
    {
      kuchukbaeva::LCIter< unsigned long long > itSum = sums.cbegin();
      if (itSum != sums.cend())
      {
        std::cout << *itSum;
        ++itSum;
        for (; itSum != sums.cend(); ++itSum)
        {
          std::cout << " " << *itSum;
        }
        std::cout << "\n";
      }
    }
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Fatal except: " << e.what() << "\n";
    return 2;
  }
  catch (...)
  {
    std::cerr << "Fatal error\n";
    return 2;
  }
}
