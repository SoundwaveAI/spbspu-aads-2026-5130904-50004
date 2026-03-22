#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(testQueuePushAndDrop)
{
  kuchukbaeva::Queue< int > queue;
  BOOST_CHECK(queue.isEmpty());
  queue.push(10);
  queue.push(20);
  BOOST_CHECK(!queue.isEmpty());
  BOOST_CHECK_EQUAL(queue.drop(), 10);
  BOOST_CHECK_EQUAL(queue.drop(), 20);
  BOOST_CHECK(queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(testQueueException)
{
  kuchukbaeva::Queue< int > queue;
  BOOST_CHECK_THROW(queue.drop(), std::out_of_range);
}
