#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(testStackPushAndDrop) {
  kuchukbaeva::Stack< int > stack;
  BOOST_CHECK(stack.isEmpty());
  stack.push(10);
  stack.push(20);
  BOOST_CHECK(!stack.isEmpty());
  BOOST_CHECK_EQUAL(stack.top(), 20);
  stack.drop();
  BOOST_CHECK_EQUAL(stack.top(), 10);
  stack.drop();
  BOOST_CHECK(stack.isEmpty());
}

BOOST_AUTO_TEST_CASE(testStackException) {
  kuchukbaeva::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::out_of_range);
  BOOST_CHECK_THROW(stack.drop(), std::out_of_range);
}
