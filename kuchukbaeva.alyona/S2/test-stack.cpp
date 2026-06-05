#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(testStackPush)
{
  kuchukbaeva::Stack< int > stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.getSize(), 0);

  stack.push(10);
  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.getSize(), 1);
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.push(20);
  BOOST_CHECK_EQUAL(stack.getSize(), 2);
  BOOST_CHECK_EQUAL(stack.top(), 20);
}

BOOST_AUTO_TEST_CASE(testStackPop)
{
  kuchukbaeva::Stack< int > stack;
  stack.push(10);
  stack.push(20);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.getSize(), 1);
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.pop();
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testStackClear)
{
  kuchukbaeva::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);

  stack.clear();

  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testStackConstTop)
{
  kuchukbaeva::Stack< int > stack;
  stack.push(42);
  const kuchukbaeva::Stack< int >& constStack = stack;

  BOOST_CHECK_EQUAL(constStack.top(), 42);
}

BOOST_AUTO_TEST_CASE(testStackException) {
  kuchukbaeva::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::out_of_range);
  BOOST_CHECK_THROW(stack.pop(), std::out_of_range);

  const kuchukbaeva::Stack< int >& constStack = stack;
  BOOST_CHECK_THROW(constStack.top(), std::out_of_range);
}
