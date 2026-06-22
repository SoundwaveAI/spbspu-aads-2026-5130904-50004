#include <boost/test/unit_test.hpp>
#include "tour.hpp"
#include <string>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(testTournamentDefaultConstructor)
{
  kuchukbaeva::Tournament t;
  BOOST_CHECK_EQUAL(t.getName(), "");
  BOOST_CHECK(!t.isRoundActive());
}

BOOST_AUTO_TEST_CASE(testTournamentParamConstructor)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 10);
  BOOST_CHECK_EQUAL(t.getName(), "Test");
  BOOST_CHECK(t.getType() == kuchukbaeva::TournamentType::SWISS);
}

BOOST_AUTO_TEST_CASE(testTournamentCopyConstructor)
{
  kuchukbaeva::Tournament orig("Test", kuchukbaeva::TournamentType::OLYMPIC, 4);
  kuchukbaeva::Tournament copy(orig);
  BOOST_CHECK_EQUAL(copy.getName(), "Test");
}

BOOST_AUTO_TEST_CASE(testTournamentMoveConstructor)
{
  kuchukbaeva::Tournament orig("Test", kuchukbaeva::TournamentType::OLYMPIC, 4);
  kuchukbaeva::Tournament moved(std::move(orig));
  BOOST_CHECK_EQUAL(moved.getName(), "Test");
}

BOOST_AUTO_TEST_CASE(testTournamentCopyAssignment)
{
  kuchukbaeva::Tournament orig("Test", kuchukbaeva::TournamentType::OLYMPIC, 4);
  kuchukbaeva::Tournament copy;
  copy = orig;
  BOOST_CHECK_EQUAL(copy.getName(), "Test");
}

BOOST_AUTO_TEST_CASE(testTournamentMoveAssignment)
{
  kuchukbaeva::Tournament orig("Test", kuchukbaeva::TournamentType::OLYMPIC, 4);
  kuchukbaeva::Tournament moved;
  moved = std::move(orig);
  BOOST_CHECK_EQUAL(moved.getName(), "Test");
}

BOOST_AUTO_TEST_CASE(testTournamentSwap)
{
  kuchukbaeva::Tournament t1("T1", kuchukbaeva::TournamentType::OLYMPIC, 2);
  kuchukbaeva::Tournament t2("T2", kuchukbaeva::TournamentType::SWISS, 4);
  t1.swap(t2);
  BOOST_CHECK_EQUAL(t1.getName(), "T2");
  BOOST_CHECK_EQUAL(t2.getName(), "T1");
}

BOOST_AUTO_TEST_CASE(testTournamentJoinPlayer)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("P1", 1500);
  BOOST_CHECK_EQUAL(t.getPlayerStatus("P1").rating, 1500);
}

BOOST_AUTO_TEST_CASE(testTournamentLeavePlayer)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 11);
  for (int i = 0; i < 11; ++i) {
      t.joinPlayer("P" + std::to_string(i), 1000);
  }
  t.leavePlayer("P0");
  BOOST_CHECK_THROW(t.getPlayerStatus("P0"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(testTournamentSubstitutePlayer)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("Old", 1500);
  t.substitutePlayer("Old", "New", 1600);
  BOOST_CHECK_EQUAL(t.getPlayerStatus("New").rating, 1600);
  BOOST_CHECK_THROW(t.getPlayerStatus("Old"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(testTournamentStartRoundOlympic)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  BOOST_CHECK(t.isRoundActive());
}

BOOST_AUTO_TEST_CASE(testTournamentStartRoundSwiss)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  BOOST_CHECK(t.isRoundActive());
}

BOOST_AUTO_TEST_CASE(testTournamentRecordMatch)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  t.recordMatch("P1", "P2", 1.0f);
  BOOST_CHECK(t.getPlayerStatus("P2").isEliminated);
}

BOOST_AUTO_TEST_CASE(testTournamentNextRound)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  t.recordMatch("P1", "P2", 1.0f);
  t.nextRound();
  BOOST_CHECK(!t.isRoundActive());
  BOOST_CHECK_EQUAL(t.getCurrentRound(), 1);
}

BOOST_AUTO_TEST_CASE(testTournamentGetName)
{
  kuchukbaeva::Tournament t("GetNameTest", kuchukbaeva::TournamentType::SWISS, 2);
  BOOST_CHECK_EQUAL(t.getName(), "GetNameTest");
}

BOOST_AUTO_TEST_CASE(testTournamentGetType)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  BOOST_CHECK(t.getType() == kuchukbaeva::TournamentType::OLYMPIC);
}

BOOST_AUTO_TEST_CASE(testTournamentGetGraph)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("P1", 1000);
  BOOST_CHECK(t.getGraph().hasPlayer("P1"));
}

BOOST_AUTO_TEST_CASE(testTournamentGetPlayerStatus)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("P1", 1500);
  kuchukbaeva::ParticipantStatus stat = t.getPlayerStatus("P1");
  BOOST_CHECK_EQUAL(stat.rating, 1500);
}

BOOST_AUTO_TEST_CASE(testTournamentGetCurrentPairs)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  BOOST_CHECK_EQUAL(t.getCurrentPairs().getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testTournamentIsRoundActive)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  BOOST_CHECK(!t.isRoundActive());
}

BOOST_AUTO_TEST_CASE(testTournamentGetOpponentsScores)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  t.recordMatch("P1", "P2", 1.0f);
  kuchukbaeva::Vector< float > scores = t.getOpponentsScores("P1");
  BOOST_CHECK_EQUAL(scores.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(testTournamentIsPlayerBetter)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  t.recordMatch("P1", "P2", 1.0f);
  BOOST_CHECK(t.isPlayerBetter("P1", "P2"));
}

BOOST_AUTO_TEST_CASE(testTournamentGetCurrentRound)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::SWISS, 2);
  BOOST_CHECK_EQUAL(t.getCurrentRound(), 0);
}

BOOST_AUTO_TEST_CASE(testTournamentGetPendingMatches)
{
  kuchukbaeva::Tournament t("Test", kuchukbaeva::TournamentType::OLYMPIC, 2);
  t.joinPlayer("P1", 1000);
  t.joinPlayer("P2", 1100);
  t.startRound();
  BOOST_CHECK_EQUAL(t.getPendingMatches().getSize(), 1);
}
