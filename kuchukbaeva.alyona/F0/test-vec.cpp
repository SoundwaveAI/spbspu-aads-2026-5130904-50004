#include <boost/test/unit_test.hpp>
#include <vector.hpp>
#include <stdexcept>
#include <utility>

BOOST_AUTO_TEST_CASE(testVectorDefaultConstructor)
{
  kuchukbaeva::Vector< int > vec;
  BOOST_CHECK(vec.isEmpty());
  BOOST_CHECK_EQUAL(vec.getSize(), 0);
  BOOST_CHECK_EQUAL(vec.getCapacity(), 0);
}

BOOST_AUTO_TEST_CASE(testVectorSizeConstructor)
{
  kuchukbaeva::Vector< int > vec(10);
  BOOST_CHECK(vec.isEmpty());
  BOOST_CHECK_EQUAL(vec.getCapacity(), 10);
}

BOOST_AUTO_TEST_CASE(testVectorCopyConstructor)
{
  kuchukbaeva::Vector< int > orig;
  orig.pushBack(5);
  kuchukbaeva::Vector< int > copy(orig);
  BOOST_CHECK_EQUAL(copy.getSize(), 1);
  BOOST_CHECK_EQUAL(copy[0], 5);
}

BOOST_AUTO_TEST_CASE(testVectorMoveConstructor)
{
  kuchukbaeva::Vector< int > orig;
  orig.pushBack(5);
  kuchukbaeva::Vector< int > moved(std::move(orig));
  BOOST_CHECK_EQUAL(moved.getSize(), 1);
  BOOST_CHECK_EQUAL(orig.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testVectorCopyAssignment)
{
  kuchukbaeva::Vector< int > orig;
  orig.pushBack(10);
  kuchukbaeva::Vector< int > copy;
  copy = orig;
  BOOST_CHECK_EQUAL(copy.getSize(), 1);
  BOOST_CHECK_EQUAL(copy[0], 10);
}

BOOST_AUTO_TEST_CASE(testVectorMoveAssignment)
{
  kuchukbaeva::Vector< int > orig;
  orig.pushBack(10);
  kuchukbaeva::Vector< int > moved;
  moved = std::move(orig);
  BOOST_CHECK_EQUAL(moved.getSize(), 1);
  BOOST_CHECK_EQUAL(orig.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testVectorSwap)
{
  kuchukbaeva::Vector< int > v1;
  v1.pushBack(1);
  kuchukbaeva::Vector< int > v2;
  v2.pushBack(2);
  v1.swap(v2);
  BOOST_CHECK_EQUAL(v1[0], 2);
  BOOST_CHECK_EQUAL(v2[0], 1);
}

BOOST_AUTO_TEST_CASE(testVectorInsert)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(1);
  vec.pushBack(3);
  vec.insert(1, 2);
  BOOST_CHECK_EQUAL(vec[1], 2);
  BOOST_CHECK_EQUAL(vec.getSize(), 3);
}

BOOST_AUTO_TEST_CASE(testVectorErase)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(1);
  vec.pushBack(2);
  vec.erase(0);
  BOOST_CHECK_EQUAL(vec[0], 2);
  BOOST_CHECK_EQUAL(vec.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testVectorIsEmpty)
{
  kuchukbaeva::Vector< int > vec;
  BOOST_CHECK(vec.isEmpty());
  vec.pushBack(1);
  BOOST_CHECK(!vec.isEmpty());
}

BOOST_AUTO_TEST_CASE(testVectorGetSize)
{
  kuchukbaeva::Vector< int > vec;
  BOOST_CHECK_EQUAL(vec.getSize(), 0);
  vec.pushBack(1);
  BOOST_CHECK_EQUAL(vec.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testVectorGetCapacity)
{
  kuchukbaeva::Vector< int > vec;
  vec.reserve(20);
  BOOST_CHECK_EQUAL(vec.getCapacity(), 20);
}

BOOST_AUTO_TEST_CASE(testVectorPushBackLvalue)
{
  kuchukbaeva::Vector< int > vec;
  int val = 42;
  vec.pushBack(val);
  BOOST_CHECK_EQUAL(vec[0], 42);
}

BOOST_AUTO_TEST_CASE(testVectorPushBackRvalue)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(42);
  BOOST_CHECK_EQUAL(vec[0], 42);
}

BOOST_AUTO_TEST_CASE(testVectorPopBack)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(1);
  vec.popBack();
  BOOST_CHECK(vec.isEmpty());
  BOOST_CHECK_THROW(vec.popBack(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testVectorReserve)
{
  kuchukbaeva::Vector< int > vec;
  vec.reserve(50);
  BOOST_CHECK(vec.getCapacity() >= 50);
}

BOOST_AUTO_TEST_CASE(testVectorOperatorBracket)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(10);
  BOOST_CHECK_EQUAL(vec[0], 10);
  vec[0] = 20;
  BOOST_CHECK_EQUAL(vec[0], 20);
}

BOOST_AUTO_TEST_CASE(testVectorAt)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(10);
  BOOST_CHECK_EQUAL(vec.at(0), 10);
  BOOST_CHECK_THROW(vec.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testVectorBegin)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(10);
  BOOST_CHECK_EQUAL(*(vec.begin()), 10);
}

BOOST_AUTO_TEST_CASE(testVectorEnd)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(10);
  auto it = vec.begin();
  ++it;
  BOOST_CHECK(it == vec.end());
}

BOOST_AUTO_TEST_CASE(testVectorCBegin)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(10);
  BOOST_CHECK_EQUAL(*(vec.cbegin()), 10);
}

BOOST_AUTO_TEST_CASE(testVectorCEnd)
{
  kuchukbaeva::Vector< int > vec;
  vec.pushBack(10);
  auto it = vec.cbegin();
  ++it;
  BOOST_CHECK(it == vec.cend());
}
