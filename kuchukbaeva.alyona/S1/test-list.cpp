#include <boost/test/unit_test.hpp>
#include <list.hpp>

BOOST_AUTO_TEST_CASE(testInsertAfter)
{
  kuchukbaeva::List< int > list;
  BOOST_CHECK(list.isEmpty());

  kuchukbaeva::LIter< int > it = list.beforeBegin();
  it = list.insertAfter(it, 10);
  BOOST_CHECK(!list.isEmpty());
  BOOST_CHECK_EQUAL(list.front(), 10);

  it = list.insertAfter(it, 20);
  list.insertAfter(it, 30);

  BOOST_CHECK(!list.isEmpty());
}


BOOST_AUTO_TEST_CASE(testIteratorTraversal)
{
  kuchukbaeva::List< int > list;
  kuchukbaeva::LIter< int > it = list.beforeBegin();
  it = list.insertAfter(it, 10);
  it = list.insertAfter(it, 20);
  list.insertAfter(it, 30);

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

BOOST_AUTO_TEST_CASE(testFront)
{
  kuchukbaeva::List< int > list;
  list.push_front(10);
  auto it = list.begin();
  list.insertAfter(it, 20);
  BOOST_CHECK_EQUAL(list.front(), 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(testPushPop)
{
  kuchukbaeva::List< int > list;
  auto it = list.beforeBegin();
  it = list.insertAfter(it, 1);
  list.insertAfter(it, 2);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.front(), 0);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.front(), 1);
  list.eraseAfter(list.begin());
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK(++list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(testClear)
{
  kuchukbaeva::List< int > list;
  list.push_front(1);
  list.push_front(2);
  BOOST_CHECK(!list.isEmpty());
  list.clear();
  BOOST_CHECK(list.isEmpty());
}

BOOST_AUTO_TEST_CASE(testCopyMove)
{
  kuchukbaeva::List< int > list1;
  list1.push_front(3);
  list1.push_front(2);
  list1.push_front(1);
  kuchukbaeva::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  kuchukbaeva::List< int > list3(std::move(list1));
  BOOST_CHECK_EQUAL(list3.front(), 1);
  BOOST_CHECK(list1.isEmpty());
}
