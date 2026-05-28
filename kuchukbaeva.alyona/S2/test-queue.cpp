#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(testQueuePush)
{
  kuchukbaeva::Queue< int > queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.getSize(), 0);

  queue.push(10);
  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.getSize(), 1);
  BOOST_CHECK_EQUAL(queue.top(), 10);

  queue.push(20);
  BOOST_CHECK_EQUAL(queue.getSize(), 2);
  BOOST_CHECK_EQUAL(queue.top(), 10);
}

BOOST_AUTO_TEST_CASE(testQueuePop)
{
  kuchukbaeva::Queue< int > queue;
  queue.push(10);
  queue.push(20);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.getSize(), 1);
  BOOST_CHECK_EQUAL(queue.top(), 20);

  queue.pop();
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testQueueClear)
{
  kuchukbaeva::Queue< int > queue;
  queue.push(1);
  queue.push(2);

  queue.clear();

  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.getSize(), 0);

  queue.push(3);
  BOOST_CHECK_EQUAL(queue.top(), 3);
  BOOST_CHECK_EQUAL(queue.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testQueueConstTop)
{
  kuchukbaeva::Queue< int > queue;
  queue.push(42);
  const kuchukbaeva::Queue< int >& constQueue = queue;

  BOOST_CHECK_EQUAL(constQueue.top(), 42);
}

BOOST_AUTO_TEST_CASE(testQueueException) {
  kuchukbaeva::Queue< int > queue;
  BOOST_CHECK_THROW(queue.top(), std::out_of_range);
  BOOST_CHECK_THROW(queue.pop(), std::out_of_range);

  const kuchukbaeva::Queue< int >& constQueue = queue;
  BOOST_CHECK_THROW(constQueue.top(), std::out_of_range);
}
