#include <boost/test/unit_test.hpp>
#include "bsTree.hpp"
#include <string>
#include <stdexcept>

using namespace kuchukbaeva;
using Tree = BSTree< int, std::string >;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  Tree t;
  BOOST_CHECK(t.isEmpty());
  BOOST_CHECK_EQUAL(t.getSize(), 0);
  BOOST_CHECK_EQUAL(t.height(), 0);
}

BOOST_AUTO_TEST_CASE(PushAndGet)
{
  Tree t;
  t.push(10, "ten");
  BOOST_CHECK(!t.isEmpty());
  BOOST_CHECK_EQUAL(t.getSize(), 1);
  BOOST_CHECK_EQUAL(t.get(10), "ten");

  t.push(10, "TEN");
  BOOST_CHECK_EQUAL(t.getSize(), 1);
  BOOST_CHECK_EQUAL(t.get(10), "TEN");

  t.push(5, "five");
  t.push(15, "fifteen");
  BOOST_CHECK_EQUAL(t.getSize(), 3);
  BOOST_CHECK_EQUAL(t.get(5), "five");
  BOOST_CHECK_EQUAL(t.get(15), "fifteen");
}

BOOST_AUTO_TEST_CASE(GetNonexistentException)
{
  Tree t;
  BOOST_CHECK_THROW(t.get(42), std::out_of_range);

  const Tree& ct = t;
  BOOST_CHECK_THROW(ct.get(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(FindMethod)
{
  Tree t;
  t.push(10, "ten");
  t.push(20, "twenty");

  auto it = t.find(20);
  BOOST_CHECK(it != t.end());
  BOOST_CHECK_EQUAL(it->first, 20);
  BOOST_CHECK_EQUAL(it->second, "twenty");

  auto fakeIt = t.find(99);
  BOOST_CHECK(fakeIt == t.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructorAndAssignment)
{
  Tree t1;
  t1.push(2, "two");
  t1.push(1, "one");
  t1.push(3, "three");

  Tree t2(t1);
  BOOST_CHECK_EQUAL(t2.getSize(), t1.getSize());
  BOOST_CHECK_EQUAL(t2.get(1), "one");
  BOOST_CHECK_EQUAL(t2.get(2), "two");
  BOOST_CHECK_EQUAL(t2.get(3), "three");

  Tree t3;
  t3.push(10, "ten");
  t3 = t1;
  BOOST_CHECK_EQUAL(t3.getSize(), 3);
  BOOST_CHECK_EQUAL(t3.get(1), "one");
  BOOST_CHECK_THROW(t3.get(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(MoveConstructorAndAssignment)
{
  Tree t1;
  t1.push(5, "five");

  Tree t2(std::move(t1));
  BOOST_CHECK_EQUAL(t2.getSize(), 1);
  BOOST_CHECK_EQUAL(t2.get(5), "five");
  BOOST_CHECK(t1.isEmpty());

  Tree t3;
  t3 = std::move(t2);
  BOOST_CHECK_EQUAL(t3.getSize(), 1);
  BOOST_CHECK(t2.isEmpty());
}

BOOST_AUTO_TEST_CASE(DropMethod)
{
  Tree t;
  t.push(10, "10");
  t.push(5, "5");
  t.push(15, "15");
  t.push(3, "3");

  BOOST_CHECK_EQUAL(t.drop(3), "3");
  BOOST_CHECK_EQUAL(t.getSize(), 3);
  BOOST_CHECK(t.find(3) == t.end());

  t.push(7, "7");
  BOOST_CHECK_EQUAL(t.drop(5), "5");
  BOOST_CHECK_EQUAL(t.get(7), "7");

  BOOST_CHECK_EQUAL(t.drop(10), "10");
  BOOST_CHECK_EQUAL(t.getSize(), 2);
  BOOST_CHECK_THROW(t.drop(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(IteratorTraversal)
{
  Tree t;
  t.push(10, "10");
  t.push(5, "5");
  t.push(15, "15");

  auto it = t.begin();
  BOOST_CHECK_EQUAL(it->first, 5);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 10);
  it++;
  BOOST_CHECK_EQUAL(it->first, 15);
  ++it;
  BOOST_CHECK(it == t.end());

  auto last_it = t.find(15);
  --last_it;
  BOOST_CHECK_EQUAL(last_it->first, 10);
}

BOOST_AUTO_TEST_CASE(RotateLeftTest)
{
  Tree t;
  t.push(10, "10");
  t.push(20, "20");
  t.push(30, "30");
  BOOST_CHECK_EQUAL(t.height(), 3);

  auto it = t.find(20);
  t.rotateLeft(it);
  BOOST_CHECK_EQUAL(t.height(), 2);
  BOOST_CHECK_EQUAL(t.get(10), "10");
  BOOST_CHECK_EQUAL(t.get(20), "20");
  BOOST_CHECK_EQUAL(t.get(30), "30");
}

BOOST_AUTO_TEST_CASE(RotateRightTest)
{
  Tree t;
  t.push(30, "30");
  t.push(20, "20");
  t.push(10, "10");
  BOOST_CHECK_EQUAL(t.height(), 3);

  auto it = t.find(20);
  t.rotateRight(it);
  BOOST_CHECK_EQUAL(t.height(), 2);
  BOOST_CHECK_EQUAL(t.get(10), "10");
}

BOOST_AUTO_TEST_CASE(RotateLargeLeftTest)
{
  Tree t;
  t.push(10, "10");
  t.push(30, "30");
  t.push(20, "20");

  auto it = t.find(20);
  t.rotateLargeLeft(it);
  BOOST_CHECK_EQUAL(t.height(), 2);
  BOOST_CHECK_EQUAL(t.get(10), "10");
  BOOST_CHECK_EQUAL(t.get(20), "20");
  BOOST_CHECK_EQUAL(t.get(30), "30");
}

BOOST_AUTO_TEST_CASE(RotateLargeRightTest)
{
  Tree t;
  t.push(30, "30");
  t.push(10, "10");
  t.push(20, "20");

  auto it = t.find(20);
  t.rotateLargeRight(it);
  BOOST_CHECK_EQUAL(t.height(), 2);
  BOOST_CHECK_EQUAL(t.get(10), "10");
  BOOST_CHECK_EQUAL(t.get(20), "20");
  BOOST_CHECK_EQUAL(t.get(30), "30");
}
