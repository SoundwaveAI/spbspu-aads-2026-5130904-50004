#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char* argv[])
#endif

#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(testInsertAndIterate)
{
  kuchukbaeva::List< int > list;
  BOOST_CHECK(list.isEmpty());

  kuchukbaeva::LIter< int > it = list.beforeBegin();
  it = list.insertAfter(it, 10);
  it = list.insertAfter(it, 20);
  list.insertAfter(it, 30);

  BOOST_CHECK(!list.isEmpty());

  kuchukbaeva::LIter< int > iter = list.begin();
  BOOST_CHECK_EQUAL(*iter, 10);
  ++iter;
  BOOST_CHECK_EQUAL(*iter, 20);
  ++iter;
  BOOST_CHECK_EQUAL(*iter, 30);
  ++iter;
  BOOST_CHECK(iter == list.end());
}

BOOST_AUTO_TEST_CASE(testEraseAfter)
{
  kuchukbaeva::List< int > list;
  kuchukbaeva::LIter< int > it = list.beforeBegin();
  it = list.insertAfter(it, 1);
  list.insertAfter(it, 2);

  list.eraseAfter(list.beforeBegin());

  kuchukbaeva::LIter< int > iter = list.begin();
  BOOST_CHECK_EQUAL(*iter, 2);
  ++iter;
  BOOST_CHECK(iter == list.end());
}

BOOST_AUTO_TEST_CASE(testClearAndCopy)
{
  kuchukbaeva::List< int > listOne;
  listOne.insertAfter(listOne.beforeBegin(), 42);

  kuchukbaeva::List< int > listTwo = listOne;
  BOOST_CHECK_EQUAL(*(listTwo.begin()), 42);

  listOne.clear();
  BOOST_CHECK(listOne.isEmpty());
  BOOST_CHECK(!listTwo.isEmpty());
}
