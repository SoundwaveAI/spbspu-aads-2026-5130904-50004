#if defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) int main(int argc, char* argv[]);
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

BOOST_AUTO_TEST_CASE(testFrontBack)
{
  kuchukbaeva::List< int > list;
  list.push_front(10);
  list.push_back(20);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 20);
}

BOOST_AUTO_TEST_CASE(testPushPop)
{
  kuchukbaeva::List< int > list;
  list.push_front(1);
  list.push_back(2);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.front(), 0);
  BOOST_CHECK_EQUAL(list.back(), 2);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.front(), 1);
  list.pop_back();
  BOOST_CHECK_EQUAL(list.back(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(testClear)
{
  kuchukbaeva::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_CHECK(!list.isEmpty());
  list.clear();
  BOOST_CHECK(list.isEmpty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(testCopyMove)
{
  kuchukbaeva::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  kuchukbaeva::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK_EQUAL(list2.back(), 2);
  kuchukbaeva::List< int > list3(std::move(list1));
  BOOST_CHECK_EQUAL(list3.front(), 1);
  BOOST_CHECK(list1.isEmpty());
}
