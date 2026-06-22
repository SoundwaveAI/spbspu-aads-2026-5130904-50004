#include <boost/test/unit_test.hpp>
#include "graph.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(testGraphDefaultConstructor)
{
  kuchukbaeva::Graph g;
  BOOST_CHECK_EQUAL(g.getPlayers().getSize(), 0);
}

BOOST_AUTO_TEST_CASE(testGraphCopyConstructor)
{
  kuchukbaeva::Graph orig;
  orig.addPlayer("P1");
  kuchukbaeva::Graph copy(orig);
  BOOST_CHECK(copy.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphMoveConstructor)
{
  kuchukbaeva::Graph orig;
  orig.addPlayer("P1");
  kuchukbaeva::Graph moved(std::move(orig));
  BOOST_CHECK(moved.hasPlayer("P1"));
  BOOST_CHECK(!orig.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphCopyAssignment)
{
  kuchukbaeva::Graph orig;
  orig.addPlayer("P1");
  kuchukbaeva::Graph copy;
  copy = orig;
  BOOST_CHECK(copy.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphMoveAssignment)
{
  kuchukbaeva::Graph orig;
  orig.addPlayer("P1");
  kuchukbaeva::Graph moved;
  moved = std::move(orig);
  BOOST_CHECK(moved.hasPlayer("P1"));
  BOOST_CHECK(!orig.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphSwap)
{
  kuchukbaeva::Graph g1;
  g1.addPlayer("P1");
  kuchukbaeva::Graph g2;
  g2.addPlayer("P2");
  g1.swap(g2);
  BOOST_CHECK(g1.hasPlayer("P2"));
  BOOST_CHECK(g2.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphAddPlayer)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  BOOST_CHECK(g.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphRemovePlayer)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  g.removePlayer("P1");
  BOOST_CHECK(!g.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphSubstitutePlayer)
{
  kuchukbaeva::Graph g;
  g.addPlayer("Old");
  g.substitutePlayer("Old", "New");
  BOOST_CHECK(!g.hasPlayer("Old"));
  BOOST_CHECK(g.hasPlayer("New"));
}

BOOST_AUTO_TEST_CASE(testGraphHasPlayer)
{
  kuchukbaeva::Graph g;
  BOOST_CHECK(!g.hasPlayer("P1"));
  g.addPlayer("P1");
  BOOST_CHECK(g.hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testGraphGetPlayers)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  BOOST_CHECK_EQUAL(g.getPlayers().getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testGraphAddMatch)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  g.addPlayer("P2");
  g.addMatch("P1", "P2", 1.0f);
  BOOST_CHECK(g.hasPlayed("P1", "P2"));
}

BOOST_AUTO_TEST_CASE(testGraphHasPlayed)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  g.addPlayer("P2");
  BOOST_CHECK(!g.hasPlayed("P1", "P2"));
  g.addMatch("P1", "P2", 0.5f);
  BOOST_CHECK(g.hasPlayed("P1", "P2"));
}

BOOST_AUTO_TEST_CASE(testGraphGetPlayerScore)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  g.addPlayer("P2");
  g.addMatch("P1", "P2", 1.0f);
  BOOST_CHECK_EQUAL(g.getPlayerScore("P1"), 1.0f);
  BOOST_CHECK_EQUAL(g.getPlayerScore("P2"), 0.0f);
}

BOOST_AUTO_TEST_CASE(testGraphGetOpponents)
{
  kuchukbaeva::Graph g;
  g.addPlayer("P1");
  g.addPlayer("P2");
  g.addMatch("P1", "P2", 1.0f);
  kuchukbaeva::Vector< std::string > opps = g.getOpponents("P1");
  BOOST_CHECK_EQUAL(opps.getSize(), 1);
  BOOST_CHECK_EQUAL(opps[0], "P2");
}
