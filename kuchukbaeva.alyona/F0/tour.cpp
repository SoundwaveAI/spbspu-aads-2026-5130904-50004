#include "tour.hpp"
#include <stdexcept>
#include <ostream>
#include <istream>
#include <string>

kuchukbaeva::Tournament::Tournament():
  name_(""),
  type_(TournamentType::SWISS),
  graph_(),
  statuses_(16),
  isRoundActive_(false),
  currentRound_(0),
  currentPairs_(),
  playedThisRound_(16)
{}

kuchukbaeva::Tournament::Tournament(const std::string& name,
  TournamentType type, size_t expectedPlayers):
  name_(name),
  type_(type),
  graph_(),
  statuses_(16),
  isRoundActive_(false),
  currentRound_(0),
  currentPairs_(),
  playedThisRound_(16),
  expectedPlayers_(expectedPlayers)
{
  if (type_ == TournamentType::SWISS)
  {
    graph_.addPlayer("плюсов");
  }
}

kuchukbaeva::Tournament::Tournament(const Tournament& other):
  name_(other.name_),
  type_(other.type_),
  graph_(other.graph_),
  statuses_(other.statuses_),
  isRoundActive_(other.isRoundActive_),
  currentRound_(other.currentRound_),
  currentPairs_(other.currentPairs_),
  playedThisRound_(other.playedThisRound_),
  expectedPlayers_(other.expectedPlayers_)
{}

kuchukbaeva::Tournament::Tournament(Tournament&& other) noexcept:
  name_(std::move(other.name_)),
  type_(other.type_),
  graph_(std::move(other.graph_)),
  statuses_(std::move(other.statuses_)),
  isRoundActive_(other.isRoundActive_),
  currentRound_(other.currentRound_),
  currentPairs_(std::move(other.currentPairs_)),
  playedThisRound_(std::move(other.playedThisRound_)),
  expectedPlayers_(other.expectedPlayers_)
{
  other.expectedPlayers_ = 0;
}

kuchukbaeva::Tournament& kuchukbaeva::Tournament::operator=(const Tournament& other)
{
  if (this != std::addressof(other))
  {
    Tournament temp(other);
    swap(temp);
  }
  return *this;
}

kuchukbaeva::Tournament& kuchukbaeva::Tournament::operator=(Tournament&& other) noexcept
{
  if (this != std::addressof(other))
  {
    swap(other);
  }
  return *this;
}

void kuchukbaeva::Tournament::swap(Tournament& other) noexcept
{
  std::swap(name_, other.name_);
  std::swap(type_, other.type_);
  graph_.swap(other.graph_);
  statuses_.swap(other.statuses_);
  std::swap(isRoundActive_, other.isRoundActive_);
  std::swap(currentRound_, other.currentRound_);
  currentPairs_.swap(other.currentPairs_);
  playedThisRound_.swap(other.playedThisRound_);
  std::swap(expectedPlayers_, other.expectedPlayers_);
}

void kuchukbaeva::Tournament::joinPlayer(const std::string& playerName, int rating)
{
  if (currentRound_ > 0 || isRoundActive_)
  {
    throw TournamentException("Турнир уже начат, добавление игроков невозможно");
  }
  if (graph_.hasPlayer(playerName))
  {
    throw TournamentException("Игрок уже добавлен в этот турнир");
  }
  Tournament temp(*this);
  temp.graph_.addPlayer(playerName);
  ParticipantStatus initialStatus;
  initialStatus.isEliminated = false;
  initialStatus.currentStage = 0;
  initialStatus.rating = rating;

  temp.statuses_.add(playerName, initialStatus);
  swap(temp);
}

void kuchukbaeva::Tournament::leavePlayer(const std::string& playerName)
{
  if (isRoundActive_ || currentRound_ > 0)
  {
    throw TournamentException("Турнир начат, удаление игрока невозможно");
  }
  if (!graph_.hasPlayer(playerName))
  {
    throw InvalidCommandException("");
  }
  size_t realCount = graph_.getPlayers().getSize();
  if (type_ == TournamentType::SWISS && graph_.hasPlayer("плюсов"))
  {
    realCount--;
  }

  if (realCount <= 10)
  {
    throw TournamentException("При удалении в турнире останется меньше 10 игроков");
  }

  Tournament temp(*this);
  temp.graph_.removePlayer(playerName);
  ParticipantStatus dummy;
  temp.statuses_.drop(playerName, dummy);
  swap(temp);
}

void kuchukbaeva::Tournament::substitutePlayer(const std::string& oldName,
  const std::string& newName, int newRating)
{
  if (currentRound_ > 0 || isRoundActive_)
  {
    throw TournamentException("Замена игроков невозможна, турнир уже начат");
  }

  Tournament temp(*this);
  if (!statuses_.has(oldName))
  {
    throw InvalidCommandException("");
  }
  ParticipantStatus oldStatus;
  temp.statuses_.drop(oldName, oldStatus);
  temp.graph_.substitutePlayer(oldName, newName);
  oldStatus.rating = newRating;
  temp.statuses_.add(newName, oldStatus);
  swap(temp);
}

void kuchukbaeva::Tournament::startRound()
{
  if (isRoundActive_)
  {
    throw TournamentException("Раунд уже активен");
  }
  size_t currentCount = graph_.getPlayers().getSize();
  if (type_ == TournamentType::SWISS && graph_.hasPlayer("плюсов"))
  {
    currentCount--;
  }
  if (currentCount > expectedPlayers_)
  {
    throw TournamentException("Слишком много игроков добавлено");
  }
  if ((type_ == TournamentType::OLYMPIC && currentCount != expectedPlayers_) ||
      (type_ == TournamentType::SWISS && currentCount != expectedPlayers_))
  {
    throw TournamentException("Требуется заявленное количество игроков");
  }
  if (type_ == TournamentType::SWISS)
  {
    const size_t maxRounds = 7;
    if (currentRound_ >= maxRounds)
    {
      throw TournamentException("Турнир завершен");
    }
  }
  Tournament temp(*this);
  temp.isRoundActive_ = true;
  temp.currentPairs_ = Vector< std::pair< std::string, std::string > >();
  temp.playedThisRound_ = HashTable< std::pair< std::string, std::string >,
    bool, EdgeHash, EdgeEqual >(16);

  if (temp.type_ == TournamentType::OLYMPIC)
  {
    temp.generateOlympicPairs();
  }
  else
  {
    temp.generateSwissPairs();
  }
  if (temp.currentPairs_.getSize() == 0)
  {
    throw TournamentException("Турнир завершен");
  }
  swap(temp);
}

void kuchukbaeva::Tournament::generateOlympicPairs()
{
  const Vector< std::string >& players = graph_.getPlayers();

  if (currentRound_ == 0)
  {
    Vector< std::string > activePlayers;
    for (size_t i = 0; i < players.getSize(); ++i)
    {
      if (statuses_.has(players[i]))
      {
        activePlayers.pushBack(players[i]);
      }
    }
    for (size_t i = 1; i < activePlayers.getSize(); ++i)
    {
      std::string key = activePlayers[i];
      int keyRating = statuses_.find(key)->second.rating;
      int j = static_cast< int >(i) - 1;
      while (j >= 0)
      {
        int currentRating = statuses_.find(activePlayers[j])->second.rating;
        if (currentRating < keyRating)
        {
          activePlayers[j + 1] = activePlayers[j];
          j--;
        }
        else
        {
          break;
        }
      }
      activePlayers[j + 1] = key;
    }
    size_t numPairs = activePlayers.getSize() / 2;
    for (size_t i = 0; i < numPairs; ++i)
    {
      std::string p1 = activePlayers[i];
      std::string p2 = activePlayers[activePlayers.getSize() - 1 - i];
      currentPairs_.pushBack(std::make_pair(p1, p2));
      playedThisRound_.add(std::make_pair(p1, p2), false);
      playedThisRound_.add(std::make_pair(p2, p1), false);

      statuses_.find(p1)->second.seed = i;
      statuses_.find(p2)->second.seed = i;
    }
  }
  else
  {
    Vector< std::string > winners;
    for (size_t i = 0; i < players.getSize(); ++i)
    {
      if (statuses_.has(players[i]))
      {
        if (!statuses_.find(players[i])->second.isEliminated)
        {
          winners.pushBack(players[i]);
        }
      }
    }
    for (size_t i = 1; i < winners.getSize(); ++i)
    {
      std::string key = winners[i];
      size_t keySeed = statuses_.find(key)->second.seed;
      int j = static_cast< int >(i) - 1;
      while (j >= 0)
      {
        size_t currentSeed = statuses_.find(winners[j])->second.seed;
        if (currentSeed > keySeed)
        {
          winners[j + 1] = winners[j];
          j--;
        }
        else
        {
          break;
        }
      }
      winners[j + 1] = key;
    }

    size_t numPairs = winners.getSize() / 2;
    for (size_t i = 0; i < numPairs; ++i)
    {
      std::string p1 = winners[2 * i];
      std::string p2 = winners[2 * i + 1];

      currentPairs_.pushBack(std::make_pair(p1, p2));
      playedThisRound_.add(std::make_pair(p1, p2), false);
      playedThisRound_.add(std::make_pair(p2, p1), false);
      statuses_.find(p1)->second.seed = i;
      statuses_.find(p2)->second.seed = i;
    }
  }
}

void kuchukbaeva::Tournament::generateSwissPairs()
{
  const Vector< std::string >& allPlayers = graph_.getPlayers();
  Vector< std::string > activePlayers;
  for (size_t i = 0; i < allPlayers.getSize(); ++i)
  {
    if (allPlayers[i] != "плюсов")
    {
      activePlayers.pushBack(allPlayers[i]);
    }
  }
  if (activePlayers.getSize() % 2 != 0)
  {
    activePlayers.pushBack("плюсов");
  }

  const size_t n = activePlayers.getSize();
  if (n < 2)
  {
    return;
  }

  struct PlayerSortData
  {
    std::string name;
    float score = 0.0f;
    float buchholz = 0.0f;
    int rating = 0;
  };

  Vector< PlayerSortData > sortData;
  for (size_t i = 0; i < n; ++i)
  {
    PlayerSortData data;
    data.name = activePlayers[i];
    if (data.name == "плюсов")
    {
      data.score = -1.0f;
      data.buchholz = -1.0f;
      data.rating = -1;
    }
    else
    {
      data.score = graph_.getPlayerScore(data.name);
      Vector< float > opps = getOpponentsScores(data.name);
      for (size_t k = 0; k < opps.getSize(); ++k)
      {
        data.buchholz += opps[k];
      }
      data.rating = statuses_.find(data.name)->second.rating;
    }
    sortData.pushBack(data);
  }
  for (size_t i = 0; i < n - 1; ++i)
  {
    for (size_t j = 0; j < n - i - 1; ++j)
    {
      bool swapNeeded = false;
      if (currentRound_ == 0)
      {
        if (sortData[j].rating < sortData[j + 1].rating)
        {
          swapNeeded = true;
        }
      }
      else
      {
        if (sortData[j].score < sortData[j + 1].score)
        {
          swapNeeded = true;
        }
        else if (sortData[j].score == sortData[j + 1].score)
        {
          if (sortData[j].buchholz < sortData[j + 1].buchholz)
          {
            swapNeeded = true;
          }
          else if (sortData[j].buchholz == sortData[j + 1].buchholz)
          {
            if (sortData[j].rating < sortData[j + 1].rating)
            {
              swapNeeded = true;
            }
            else if (sortData[j].rating == sortData[j + 1].rating)
            {
              if (sortData[j].name > sortData[j + 1].name)
              {
                swapNeeded = true;
              }
            }
          }
        }
      }
      if (swapNeeded)
      {
        PlayerSortData temp = sortData[j];
        sortData[j] = sortData[j + 1];
        sortData[j + 1] = temp;
      }
    }
  }

  for (size_t i = 0; i < n; ++i)
  {
    activePlayers[i] = sortData[i].name;
  }
  Vector< char > paired;
  for (size_t i = 0; i < n; ++i)
  {
    paired.pushBack(0);
  }
  Vector< std::pair< std::string, std::string > > finalPairs;
  HashTable< std::pair< std::string, std::string >, bool, EdgeHash, EdgeEqual > finalPlayed(16);
  Graph finalGraph = graph_;

  if (currentRound_ == 0)
  {
    size_t left = 0;
    size_t right = n - 1;
    while (left < right)
    {
      std::pair< std::string, std::string > matchPair = std::make_pair(activePlayers[left],
        activePlayers[right]);
      finalPairs.pushBack(matchPair);

      if (activePlayers[left] == "плюсов" || activePlayers[right] == "плюсов")
      {
        finalPlayed.add(matchPair, true);
        finalPlayed.add(std::make_pair(matchPair.second, matchPair.first), true);
        std::string realPlayer = (activePlayers[left] == "плюсов") ? activePlayers[right] :
          activePlayers[left];
        finalGraph.addMatch(realPlayer, "плюсов", 1.0f);
      }
      else
      {
        finalPlayed.add(matchPair, false);
        finalPlayed.add(std::make_pair(matchPair.second, matchPair.first), false);
      }
      ++left;
      --right;
    }
  }
  else
  {
    Vector< std::pair< std::string, std::string > > tempPairs;
    bool perfectMatch = solvePairsRecursive(0, activePlayers, paired, tempPairs);
    if (!perfectMatch)
    {
      for(size_t k = 0; k < paired.getSize(); ++k)
      {
        paired[k] = 0;
      }
      tempPairs = Vector< std::pair< std::string, std::string > >();
      for (size_t i = 0; i < n; ++i)
      {
        if (paired[i] == 1)
        {
          continue;
        }
        for (size_t j = i + 1; j < n; ++j)
        {
          if (paired[j] == 0)
          {
            paired[i] = 1;
            paired[j] = 1;
            tempPairs.pushBack(std::make_pair(activePlayers[i], activePlayers[j]));
            break;
          }
        }
      }
    }
    for (size_t i = 0; i < tempPairs.getSize(); ++i)
    {
      std::pair< std::string, std::string > matchPair = tempPairs[i];
      finalPairs.pushBack(matchPair);
      if (matchPair.first == "плюсов" || matchPair.second == "плюсов")
      {
        finalPlayed.add(matchPair, true);
        finalPlayed.add(std::make_pair(matchPair.second, matchPair.first), true);
        std::string realPlayer = (matchPair.first == "плюсов") ? matchPair.second : matchPair.first;
        finalGraph.addMatch(realPlayer, "плюсов", 1.0f);
      }
      else
      {
        finalPlayed.add(matchPair, false);
        finalPlayed.add(std::make_pair(matchPair.second, matchPair.first), false);
      }
    }
  }
  currentPairs_.swap(finalPairs);
  playedThisRound_.swap(finalPlayed);
  graph_.swap(finalGraph);
}

void kuchukbaeva::Tournament::recordMatch(const std::string& p1, const std::string& p2, float result)
{
  if (!isRoundActive_)
  {
    throw TournamentException("Раунд не активен");
  }

  std::pair< std::string, std::string > matchPair = std::make_pair(p1, p2);
  if (!playedThisRound_.has(matchPair))
  {
    throw InvalidCommandException("");
  }
  if (playedThisRound_.find(matchPair)->second)
  {
    throw TournamentException("Результат этого матча уже зафиксирован");
  }

  Tournament temp(*this);
  temp.graph_.addMatch(p1, p2, result);

  if (temp.type_ == TournamentType::OLYMPIC)
  {
    if (result > 0.5f)
    {
      temp.statuses_.find(p2)->second.isEliminated = true;
      temp.statuses_.find(p1)->second.currentStage += 1;
    }
    else if (result < 0.5f)
    {
      temp.statuses_.find(p1)->second.isEliminated = true;
      temp.statuses_.find(p2)->second.currentStage += 1;
    }
  }
  temp.playedThisRound_.find(std::make_pair(p1, p2))->second = true;
  temp.playedThisRound_.find(std::make_pair(p2, p1))->second = true;
  swap(temp);
}

void kuchukbaeva::Tournament::nextRound()
{
  if (!isRoundActive_)
  {
    throw TournamentException("Нет активного раунда для завершения");
  }

  for (size_t i = 0; i < currentPairs_.getSize(); ++i)
  {
    if (!playedThisRound_.find(currentPairs_[i])->second)
    {
      throw TournamentException("Есть несыгранные матчи в текущем раунде");
    }
  }

  Tournament temp(*this);
  temp.isRoundActive_ = false;
  temp.currentRound_ += 1;
  temp.currentPairs_ = Vector< std::pair< std::string, std::string > >();
  temp.playedThisRound_ = HashTable< std::pair< std::string, std::string >,
    bool, EdgeHash, EdgeEqual >(16);
  swap(temp);
}

const std::string& kuchukbaeva::Tournament::getName() const noexcept
{
  return name_;
}

kuchukbaeva::TournamentType kuchukbaeva::Tournament::getType() const noexcept
{
  return type_;
}

const kuchukbaeva::Graph& kuchukbaeva::Tournament::getGraph() const noexcept
{
  return graph_;
}

kuchukbaeva::ParticipantStatus
  kuchukbaeva::Tournament::getPlayerStatus(const std::string& playerName) const
{
  if (!statuses_.has(playerName))
  {
    throw InvalidCommandException("");
  }
  return statuses_.find(playerName)->second;
}

const kuchukbaeva::Vector< std::pair< std::string, std::string > >&
  kuchukbaeva::Tournament::getCurrentPairs() const noexcept
{
  return currentPairs_;
}

bool kuchukbaeva::Tournament::isRoundActive() const noexcept
{
  return isRoundActive_;
}

void kuchukbaeva::Tournament::serialize(std::ostream& os) const
{
  os << name_ << " "
     << (type_ == TournamentType::SWISS ? "swiss" : "olympic") << " "
     << expectedPlayers_ << " "
     << currentRound_ << " "
     << (isRoundActive_ ? 1 : 0) << "\n";

  for (auto it = statuses_.cbegin(); it != statuses_.cend(); ++it)
  {
    os << it->first << " "
       << it->second.isEliminated << " "
       << it->second.currentStage << " "
       << it->second.rating << " "
       << it->second.seed << "\n";
  }
  os << "---\n";

  os << currentPairs_.getSize() << "\n";
  for (size_t i = 0; i < currentPairs_.getSize(); ++i)
  {
    os << currentPairs_[i].first << " " << currentPairs_[i].second << "\n";
  }
  os << "---\n";

  os << playedThisRound_.getSize() << "\n";
  for (auto it = playedThisRound_.cbegin(); it != playedThisRound_.cend(); ++it)
  {
    os << it->first.first << " " << it->first.second << " " << (it->second ? 1 : 0) << "\n";
  }
  os << "---\n";
  graph_.serialize(os);
}

bool kuchukbaeva::Tournament::solvePairsRecursive(size_t idx, const Vector< std::string >&
  activePlayers, Vector< char >& paired, Vector< std::pair< std::string, std::string > >& tempPairs)
{
  while (idx < activePlayers.getSize() && paired[idx] == 1)
  {
    idx++;
  }
  if (idx == activePlayers.getSize())
  {
    return true;
  }

  for (size_t j = idx + 1; j < activePlayers.getSize(); ++j)
  {
    if (paired[j] == 0)
    {
      bool isBye = (activePlayers[idx] == "плюсов" || activePlayers[j] == "плюсов");

      if (isBye || !graph_.hasPlayed(activePlayers[idx], activePlayers[j]))
      {
        paired[idx] = 1;
        paired[j] = 1;
        tempPairs.pushBack(std::make_pair(activePlayers[idx], activePlayers[j]));
        if (solvePairsRecursive(idx + 1, activePlayers, paired, tempPairs))
        {
          return true;
        }
        tempPairs.popBack();
        paired[idx] = 0;
        paired[j] = 0;
      }
    }
  }
  return false;
}

kuchukbaeva::Vector< float > kuchukbaeva::Tournament::getOpponentsScores(const std::string& player) const
{
  Vector< float > scores;
  Vector< std::string > opps = graph_.getOpponents(player);
  for (size_t i = 0; i < opps.getSize(); ++i)
  {
    scores.pushBack(graph_.getPlayerScore(opps[i]));
  }

  if (graph_.hasPlayed(player, "плюсов"))
  {
    scores.pushBack(graph_.getPlayerScore(player));
  }

  size_t n = scores.getSize();
  for (size_t i = 0; i + 1 < n; ++i)
  {
    for (size_t j = 0; j < n - i - 1; ++j)
    {
      if (scores[j] > scores[j + 1])
      {
        float temp = scores[j];
        scores[j] = scores[j + 1];
        scores[j + 1] = temp;
      }
    }
  }
  return scores;
}

bool kuchukbaeva::Tournament::isPlayerBetter(const std::string& p1, const std::string& p2) const
{
  float s1 = graph_.getPlayerScore(p1);
  float s2 = graph_.getPlayerScore(p2);
  if (s1 != s2)
  {
    return s1 > s2;
  }
  Vector< float > opps1 = getOpponentsScores(p1);
  Vector< float > opps2 = getOpponentsScores(p2);
  float bh1 = 0.0f, bh2 = 0.0f;
  for (size_t i = 0; i < opps1.getSize(); ++i)
  {
    bh1 += opps1[i];
  }
  for (size_t i = 0; i < opps2.getSize(); ++i)
  {
    bh2 += opps2[i];
  }
  if (bh1 != bh2)
  {
    return bh1 > bh2;
  }

  size_t minOpps = (opps1.getSize() < opps2.getSize()) ? opps1.getSize() : opps2.getSize();
  for (size_t i = 0; i < minOpps; ++i)
  {
    bh1 -= opps1[i];
    bh2 -= opps2[i];
    if (bh1 != bh2)
    {
      return bh1 > bh2;
    }
  }

  int r1 = statuses_.has(p1) ? statuses_.find(p1)->second.rating : 0;
  int r2 = statuses_.has(p2) ? statuses_.find(p2)->second.rating : 0;
  if (r1 != r2)
  {
    return r1 > r2;
  }
  return p1 < p2;
}

size_t kuchukbaeva::Tournament::getCurrentRound() const noexcept
{
  return currentRound_;
}

void kuchukbaeva::Tournament::deserialize(std::istream& is)
{
  std::string tStr;
  int activeInt;
  is >> name_ >> tStr >> expectedPlayers_ >> currentRound_ >> activeInt;
  isRoundActive_ = (activeInt == 1);
  type_ = (tStr == "swiss") ? TournamentType::SWISS : TournamentType::OLYMPIC;

  if (type_ == TournamentType::SWISS)
  {
    if (!graph_.hasPlayer("плюсов"))
    {
      graph_.addPlayer("плюсов");
    }
  }

  std::string token;
  while (is >> token)
  {
    if (token == "---")
    {
      break;
    }
    bool elim;
    size_t stage;
    int rat;
    size_t sd = 0;
    is >> elim >> stage >> rat >> sd;
    statuses_.add(token, {elim, stage, rat, sd});
  }
  size_t pairsCount = 0;
  is >> pairsCount;
  currentPairs_ = Vector< std::pair< std::string, std::string > >();
  for (size_t i = 0; i < pairsCount; ++i)
  {
    std::string p1, p2;
    is >> p1 >> p2;
    currentPairs_.pushBack(std::make_pair(p1, p2));
  }
  is >> token;

  size_t playedCount = 0;
  is >> playedCount;
  playedThisRound_ = HashTable< std::pair< std::string, std::string >, bool, EdgeHash, EdgeEqual >(16);
  for (size_t i = 0; i < playedCount; ++i)
  {
    std::string p1, p2;
    int played;
    is >> p1 >> p2 >> played;
    playedThisRound_.add(std::make_pair(p1, p2), (played == 1));
  }
  is >> token;
  graph_.deserialize(is);
}

kuchukbaeva::Vector< std::pair< std::string, std::string > >
  kuchukbaeva::Tournament::getPendingMatches() const
{
  Vector< std::pair< std::string, std::string > > pending;
  if (!isRoundActive_)
  {
    return pending;
  }
  for (size_t i = 0; i < currentPairs_.getSize(); ++i)
  {
    auto it = playedThisRound_.find(currentPairs_[i]);
    if (it != playedThisRound_.end() && !it->second)
    {
      pending.pushBack(currentPairs_[i]);
    }
  }
  return pending;
}
