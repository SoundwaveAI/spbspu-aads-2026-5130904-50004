#include <boost/test/unit_test.hpp>
#include "tabRobin.hpp"
#include <string>
#include <utility>

BOOST_AUTO_TEST_CASE(testHashTableDefaultConstructor)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  BOOST_CHECK_EQUAL(ht.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testHashTableSizeConstructor)
{
  kuchukbaeva::HashTable< std::string, int > ht(32);
  BOOST_CHECK_EQUAL(ht.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testHashTableCopyConstructor)
{
  kuchukbaeva::HashTable< std::string, int > orig;
  orig.add("A", 1);
  kuchukbaeva::HashTable< std::string, int > copy(orig);
  BOOST_CHECK(copy.has("A"));
}

BOOST_AUTO_TEST_CASE(testHashTableMoveConstructor)
{
  kuchukbaeva::HashTable< std::string, int > orig;
  orig.add("A", 1);
  kuchukbaeva::HashTable< std::string, int > moved(std::move(orig));
  BOOST_CHECK(moved.has("A"));
  BOOST_CHECK_EQUAL(orig.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testHashTableCopyAssignment)
{
  kuchukbaeva::HashTable< std::string, int > orig;
  orig.add("A", 1);
  kuchukbaeva::HashTable< std::string, int > copy;
  copy = orig;
  BOOST_CHECK(copy.has("A"));
}

BOOST_AUTO_TEST_CASE(testHashTableMoveAssignment)
{
  kuchukbaeva::HashTable< std::string, int > orig;
  orig.add("A", 1);
  kuchukbaeva::HashTable< std::string, int > moved;
  moved = std::move(orig);
  BOOST_CHECK(moved.has("A"));
  BOOST_CHECK_EQUAL(orig.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testHashTableSwap)
{
  kuchukbaeva::HashTable< std::string, int > ht1;
  ht1.add("A", 1);
  kuchukbaeva::HashTable< std::string, int > ht2;
  ht2.add("B", 2);
  ht1.swap(ht2);
  BOOST_CHECK(ht1.has("B"));
  BOOST_CHECK(ht2.has("A"));
}

BOOST_AUTO_TEST_CASE(testHashTableAdd)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  ht.add("Key", 100);
  BOOST_CHECK(ht.has("Key"));
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testHashTableDrop)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  ht.add("Key", 100);
  int val = 0;
  bool dropped = ht.drop("Key", val);
  BOOST_CHECK(dropped);
  BOOST_CHECK_EQUAL(val, 100);
  BOOST_CHECK(!ht.has("Key"));
}

BOOST_AUTO_TEST_CASE(testHashTableHas)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  ht.add("Key", 100);
  BOOST_CHECK(ht.has("Key"));
  BOOST_CHECK(!ht.has("Fake"));
}

BOOST_AUTO_TEST_CASE(testHashTableFind)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  ht.add("Key", 100);
  auto it = ht.find("Key");
  BOOST_CHECK(it != ht.end());
  BOOST_CHECK_EQUAL(it->second, 100);
}

BOOST_AUTO_TEST_CASE(testHashTableRehash)
{
  kuchukbaeva::HashTable< std::string, int > ht(2);
  ht.add("A", 1);
  ht.add("B", 2);
  ht.add("C", 3);
  ht.rehash(16);
  BOOST_CHECK_EQUAL(ht.getSize(), 3);
  BOOST_CHECK(ht.has("C"));
}

BOOST_AUTO_TEST_CASE(testHashTableGetSize)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  BOOST_CHECK_EQUAL(ht.getSize(), 0);
  ht.add("A", 1);
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testHashTableBeginEnd)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  ht.add("A", 1);
  auto it = ht.begin();
  BOOST_CHECK(it != ht.end());
  BOOST_CHECK_EQUAL(it->second, 1);
}

BOOST_AUTO_TEST_CASE(testHashTableCBeginCEnd)
{
  kuchukbaeva::HashTable< std::string, int > ht;
  ht.add("A", 1);
  auto it = ht.cbegin();
  BOOST_CHECK(it != ht.cend());
  BOOST_CHECK_EQUAL(it->second, 1);
}
