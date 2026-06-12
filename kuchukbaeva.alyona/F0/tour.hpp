#ifndef TOUR_HPP
#define TOUR_HPP

#include <string>
#include <utility>
#include "graph.hpp"
#include "tabRobin.hpp"
#include "vec.hpp"

namespace kuchukbaeva
{
  enum class TournamentType
  {
    SWISS,
    OLYMPIC
  };

  class TournamentException: public std::runtime_error
  {
  public:
    explicit TournamentException(const std::string& msg):
      std::runtime_error(msg)
    {}
  };

  class InvalidCommandException: public std::invalid_argument
  {
  public:
    explicit InvalidCommandException(const std::string& msg):
      std::invalid_argument(msg)
    {}
  };

  struct ParticipantStatus
  {
    bool isEliminated = false;
    size_t currentStage = 0;
    int rating = 0;
    size_t seed = 0;
  };

  class Tournament
  {
  public:
    Tournament();
    Tournament(const std::string& name, TournamentType type, size_t expectedPlayers);
    Tournament(const Tournament& other);
    Tournament(Tournament&& other) noexcept;
    ~Tournament() = default;

    Tournament& operator=(const Tournament& other);
    Tournament& operator=(Tournament&& other) noexcept;
    void swap(Tournament& other) noexcept;

    void joinPlayer(const std::string& playerName, int rating);
    void leavePlayer(const std::string& playerName);
    void substitutePlayer(const std::string& oldName, const std::string& newName, int newRating);

    void startRound();
    void recordMatch(const std::string& p1, const std::string& p2, float result);
    void nextRound();

    const std::string& getName() const noexcept;
    TournamentType getType() const noexcept;
    const Graph& getGraph() const noexcept;
    ParticipantStatus getPlayerStatus(const std::string& playerName) const;
    const Vector< std::pair< std::string, std::string > >& getCurrentPairs() const noexcept;
    bool isRoundActive() const noexcept;
    void serialize(std::ostream& os) const;
    Vector< float > getOpponentsScores(const std::string& player) const;
    bool isPlayerBetter(const std::string& p1, const std::string& p2) const;
    size_t getCurrentRound() const noexcept;
    void deserialize(std::istream& is);
    Vector< std::pair< std::string, std::string > > getPendingMatches() const;

  private:
    std::string name_;
    TournamentType type_ = TournamentType::SWISS;
    Graph graph_;
    HashTable< std::string, ParticipantStatus > statuses_;
    bool isRoundActive_ = false;
    size_t currentRound_ = 0;
    Vector< std::pair< std::string, std::string > > currentPairs_;
    HashTable< std::pair< std::string, std::string >, bool, EdgeHash, EdgeEqual > playedThisRound_;
    size_t expectedPlayers_ = 0;
    void generateOlympicPairs();
    void generateSwissPairs();
    bool solvePairsRecursive(size_t idx, const Vector< std::string >& activePlayers, Vector< char >& paired, Vector< std::pair< std::string, std::string > >& tempPairs);
  };
}

#endif
