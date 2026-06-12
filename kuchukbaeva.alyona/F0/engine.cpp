#include "eng.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <istream>
#include <string>

kuchukbaeva::Engine::Engine():
  globalDb_(),
  activeTournaments_(16)
{}

void kuchukbaeva::Engine::run()
{
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    if (line == "exit")
    {
      break;
    }
    try
    {
      processCommand(line);
    }
    catch (const kuchukbaeva::InvalidCommandException& e)
    {
      std::cerr << "<INVALID COMMAND>\n";
    }
    catch (const kuchukbaeva::TournamentException& e)
    {
      std::cerr << "<" "Подсказка: " << e.what() << ">" << "\n";
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

void kuchukbaeva::Engine::split(const std::string& line, Vector< std::string >& tokens) const
{
  size_t start = 0;
  size_t end = line.find(' ');
  while (end != std::string::npos)
  {
    if (end != start)
    {
      tokens.insert(tokens.getSize(), line.substr(start, end - start));
    }
    start = end + 1;
    end = line.find(' ', start);
  }
  if (start < line.length())
  {
    tokens.insert(tokens.getSize(), line.substr(start));
  }
}

void kuchukbaeva::Engine::processCommand(const std::string& line)
{
  Vector< std::string > tokens;
  split(line, tokens);

  if (tokens.getSize() == 0)
  {
    return;
  }

  const std::string& cmd = tokens[0];

  if (cmd == "create")
  {
    createCmd(tokens);
  }
  else if (cmd == "merge")
  {
    mergeCmd(tokens);
  }
  else if (cmd == "join")
  {
    joinCmd(tokens);
  }
  else if (cmd == "leave")
  {
    leaveCmd(tokens);
  }
  else if (cmd == "substitute")
  {
    substituteCmd(tokens);
  }
  else if (cmd == "start-round")
  {
    startRoundCmd(tokens);
  }
  else if (cmd == "match")
  {
    matchCmd(tokens);
  }
  else if (cmd == "next-round")
  {
    nextRoundCmd(tokens);
  }
  else if (cmd == "standings")
  {
    standingsCmd(tokens);
  }
  else if (cmd == "stats")
  {
    statsCmd(tokens);
  }
  else if (cmd == "save")
  {
    saveCmd(tokens);
  }
  else if (cmd == "info")
  {
    infoCmd(tokens);
  }
  else if (cmd == "top")
  {
    topCmd(tokens);
  }
  else if (cmd == "load")
  {
    loadCmd(tokens);
  }
  else if (cmd == "delete")
  {
    deleteCmd(tokens);
  }
  else
  {
    throw std::invalid_argument("Неизвестная команда");
  }
}

void kuchukbaeva::Engine::createCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 4)
  {
    throw InvalidCommandException("");
  }
  const std::string& name = tokens[1];
  const std::string& typeStr = tokens[2];
  size_t playersLimit = std::stoull(tokens[3]);

  if (playersLimit < 10)
  {
    throw InvalidCommandException("");
  }
  if (activeTournaments_.has(name))
  {
    throw TournamentException("Турнир с таким именем уже существует");
  }
  TournamentType type = TournamentType::SWISS;
  if (typeStr == "swiss")
  {
    type = TournamentType::SWISS;
  }
  else if (typeStr == "olympic")
  {
    type = TournamentType::OLYMPIC;
    if ((playersLimit & (playersLimit - 1)) != 0)
    {
      throw TournamentException("В олимпийской системе число участников должно быть степенью двойки");
    }
  }
  else
  {
    throw InvalidCommandException("");
  }

  Tournament tour(tokens[1], type, playersLimit);
  activeTournaments_.add(tokens[1], tour);
  std::cout << "<Турнир создан>\n";
}

void kuchukbaeva::Engine::mergeCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  std::ifstream file(tokens[1]);
  if (!file.is_open())
  {
    throw InvalidCommandException("");
  }
  std::string surname;
  int rating;
  while (file >> surname >> rating)
  {
    PlayerStats stats;
    if (globalDb_.hasPlayer(surname))
    {
      stats = globalDb_.getPlayerStats(surname);
    }
    stats.rating = rating;
    globalDb_.addOrUpdatePlayer(surname, stats);
  }

  if (file.fail() && !file.eof())
  {
    throw TournamentException("Неверный формат данных в файле. Ожидается: <фамилия> <рейтинг>");
  }
  std::cout << "<Успешно>\n";
}

void kuchukbaeva::Engine::joinCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 3)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  const std::string& player = tokens[2];

  if (!globalDb_.hasPlayer(player))
  {
    throw InvalidCommandException("");
  }
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  int rating = globalDb_.getPlayerStats(player).rating;
  t.joinPlayer(player, rating);
  std::cout << "<Успешно добавлен>\n";
}

void kuchukbaeva::Engine::leaveCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 3)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  const std::string& player = tokens[2];

  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  size_t realCount = t.getGraph().getPlayers().getSize();
  if (t.getType() == TournamentType::SWISS && t.getGraph().hasPlayer("плюсов"))
  {
    realCount--;
  }
  t.leavePlayer(player);
  std::cout << "<Успешно удален>\n";
}

void kuchukbaeva::Engine::substituteCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 4)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  const std::string& oldP = tokens[2];
  const std::string& newP = tokens[3];

  if (!globalDb_.hasPlayer(newP))
  {
    throw InvalidCommandException("");
  }
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  int newRating = globalDb_.getPlayerStats(newP).rating;
  t.substitutePlayer(oldP, newP, newRating);
  std::cout << "<Успешно изменено>\n";
}

void kuchukbaeva::Engine::startRoundCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  t.startRound();

  const Vector< std::pair< std::string, std::string > >& pairs = t.getCurrentPairs();
  for (size_t i = 0; i < pairs.getSize(); ++i)
  {
    std::cout << "<" << pairs[i].first << "> vs <" << pairs[i].second << ">\n";
  }
}

void kuchukbaeva::Engine::matchCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 5)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  const std::string& p1 = tokens[2];
  const std::string& p2 = tokens[3];
  float result = -1.0f;
  try
  {
    result = std::stof(tokens[4]);
  }
  catch (...)
  {
    throw InvalidCommandException("");
  }
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  Tournament tourCopy = t;
  PlayerDatabase dbCopy = globalDb_;
  if (result != 0.0f && result != 0.5f && result != 1.0f)
  {
    throw InvalidCommandException("");
  }
  if (t.getType() == TournamentType::OLYMPIC && result == 0.5f)
  {
    throw TournamentException("В олимпийской системе ничьи запрещены");
  }
  tourCopy.recordMatch(p1, p2, result);
  if (dbCopy.hasPlayer(p1))
  {
    PlayerStats s1 = dbCopy.getPlayerStats(p1);
    s1.games += 1;
    if (result == 1.0f)
    {
      s1.wins += 1;
    }
    else if (result == 0.0f)
    {
      s1.losses += 1;
    }
    else
    {
      s1.draws += 1;
    }
    dbCopy.addOrUpdatePlayer(p1, s1);
  }
  if (dbCopy.hasPlayer(p2))
  {
    PlayerStats s2 = dbCopy.getPlayerStats(p2);
    s2.games += 1;
    if (result == 0.0f)
    {
      s2.wins += 1;
    }
    else if (result == 1.0f)
    {
      s2.losses += 1;
    }
    else
    {
      s2.draws += 1;
    }
    dbCopy.addOrUpdatePlayer(p2, s2);
  }
  t.swap(tourCopy);
  globalDb_.swap(dbCopy);
  std::cout << "<Успешно зафиксированно>\n";
}

void kuchukbaeva::Engine::nextRoundCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  t.nextRound();
  std::cout << "<Раунд завершен успешно>\n";
}

void kuchukbaeva::Engine::standingsCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  const Tournament& t = activeTournaments_.find(tName)->second;
  const Vector< std::string >& players = t.getGraph().getPlayers();

  if (t.getType() == TournamentType::SWISS)
  {
    Vector< std::string > validPlayers;
    for (size_t i = 0; i < players.getSize(); ++i)
    {
      if (players[i] != "плюсов")
      {
        validPlayers.pushBack(players[i]);
      }
    }
    size_t n = validPlayers.getSize();
    for (size_t i = 0; i + 1 < n; ++i)
    {
      size_t bestIdx = i;
      for (size_t j = i + 1; j < n; ++j)
      {
        if (t.isPlayerBetter(validPlayers[j], validPlayers[bestIdx]))
        {
          bestIdx = j;
        }
      }
      if (bestIdx != i)
      {
        std::string temp = validPlayers[i];
        validPlayers[i] = validPlayers[bestIdx];
        validPlayers[bestIdx] = temp;
      }
    }
    std::cout << "Swiss:\n";
    for (size_t i = 0; i < validPlayers.getSize(); ++i)
    {
      const std::string& p = validPlayers[i];
      const float pts = t.getGraph().getPlayerScore(p);
      const int rating = globalDb_.hasPlayer(p) ? globalDb_.getPlayerStats(p).rating : 0;
      Vector< float > opps = t.getOpponentsScores(p);
      float buchholz = 0.0f;
      for (size_t k = 0; k < opps.getSize(); ++k)
      {
        buchholz += opps[k];
      }
      std::cout << " <" << i + 1 << ". " << p << ": "
                << pts << " pts (BH: " << buchholz << ", rating: " << rating << ")>\n";
    }
  }
  else
  {
    Vector< std::string > remaining;
    const Vector< std::string >& players = t.getGraph().getPlayers();

    for (size_t i = 0; i < players.getSize(); ++i)
    {
      if (!t.getPlayerStatus(players[i]).isEliminated)
      {
        remaining.pushBack(players[i]);
      }
    }
    if (t.getCurrentRound() == 0)
    {
      for (size_t i = 1; i < remaining.getSize(); ++i)
      {
        std::string key = remaining[i];
        int keyRating = t.getPlayerStatus(key).rating;
        int j = static_cast< int >(i) - 1;
        while (j >= 0)
        {
          int currentRating = t.getPlayerStatus(remaining[j]).rating;
          if (currentRating < keyRating)
          {
            remaining[j + 1] = remaining[j];
            j--;
          }
          else
          {
            break;
          }
        }
        remaining[j + 1] = key;
      }
    }
    size_t n = remaining.getSize();
    for (size_t i = 0; i + 1 < n; ++i)
    {
      for (size_t j = 0; j < n - i - 1; ++j)
      {
        size_t s1 = t.getPlayerStatus(remaining[j]).seed;
        size_t s2 = t.getPlayerStatus(remaining[j + 1]).seed;
        if (s1 > s2)
        {
          std::string temp = remaining[j];
          remaining[j] = remaining[j + 1];
          remaining[j + 1] = temp;
        }
      }
    }
    if (remaining.getSize() == 1)
    {
      std::cout << "Olympic: <Winner: " << remaining[0] << ">\n";
      return;
    }

    std::string stage;
    if (remaining.getSize() <= 2)
    {
      stage = "Final";
    }
    else if (remaining.getSize() <= 4)
    {
      stage = "1/2 Final";
    }
    else
    {
      stage = "1/" + std::to_string(remaining.getSize()/2) + " Final";
    }

    std::cout << "Olympic: <Stage: " << stage << "> <Remaining: ";
    for (size_t i = 0; i < remaining.getSize(); ++i)
    {
      std::cout << remaining[i] << (i + 1 == remaining.getSize() ? "" : ", ");
    }
    std::cout << ">\n";
  }
}

void kuchukbaeva::Engine::statsCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  const std::string& pName = tokens[1];
  if (!globalDb_.hasPlayer(pName))
  {
    throw InvalidCommandException("");
  }
  const PlayerStats& s = globalDb_.getPlayerStats(pName);
  std::cout << "<" << pName << ": games = " << s.games
            << ", wins = " << s.wins
            << ", losses = " << s.losses
            << ", draws = " << s.draws << ">\n";
}

void kuchukbaeva::Engine::saveCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  std::string filename = tokens[1];
  if (!filename.empty() && filename.back() == '\r')
  {
    filename.pop_back();
  }
  bool isEmpty = globalDb_.isEmpty() && (activeTournaments_.getSize() == 0);
  if (isEmpty)
  {
    throw InvalidCommandException("");
  }
  std::ofstream file(tokens[1]);
  if (!file.is_open())
  {
    throw InvalidCommandException("");
  }
  file << "[GLOBAL_DB]\n";
  globalDb_.serialize(file);
  file << "[TOURNAMENTS]\n";
  for (auto it = activeTournaments_.cbegin(); it != activeTournaments_.cend(); ++it)
  {
    file << "[TOUR]\n";
    it->second.serialize(file);
    file << "===\n";
  }
  file.close();
  if (file.bad())
  {
    throw std::invalid_argument("Disk error");
  }
  std::cout << "<Успешно сохранено>\n";
}

void kuchukbaeva::Engine::infoCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  if (!activeTournaments_.has(tokens[1]))
  {
    throw InvalidCommandException("");
  }
  const Tournament& t = activeTournaments_.find(tokens[1])->second;
  size_t playerCount = t.getGraph().getPlayers().getSize();
  if (t.getType() == TournamentType::SWISS && t.getGraph().hasPlayer("плюсов"))
  {
    playerCount--;
  }
  std::cout << "Name: " << t.getName() << "\n";
  std::cout << "Type: " << (t.getType() == TournamentType::SWISS ? "Swiss" : "Olympic") << "\n";
  std::cout << "Current Round: " << t.getCurrentRound() << "\n";
  std::cout << "Active Round: " << (t.isRoundActive() ? "Yes" : "No") << "\n";
  std::cout << "Players: " << playerCount << "\n";
  if (t.isRoundActive())
  {
    Vector< std::pair< std::string, std::string > > pending = t.getPendingMatches();
    if (pending.getSize() > 0)
    {
      std::cout << "Pending matches:\n";
      for (size_t i = 0; i < pending.getSize(); ++i)
      {
        std::cout << "<" << pending[i].first << ">"
                  << " vs " << "<" << pending[i].second << ">" << "\n";
      }
    }
  }
}

void kuchukbaeva::Engine::topCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  size_t n = 0;
  try
  {
    n = std::stoull(tokens[1]);
  }
  catch (...)
  {
    throw InvalidCommandException("");
  }
  if (n == 0)
  {
    throw InvalidCommandException("");
  }
  Vector< std::pair< std::string, int > > players;
  for (auto it = globalDb_.cbegin(); it != globalDb_.cend(); ++it)
  {
    players.pushBack(std::make_pair(it->first, it->second.rating));
  }
  if (players.isEmpty())
  {
    std::cout << "<Global DB is empty>\n";
    return;
  }

  size_t total = players.getSize();
  for (size_t i = 0; i + 1 < total; ++i)
  {
    size_t bestIdx = i;
    for (size_t j = i + 1; j < total; ++j)
    {
      if (players[j].second > players[bestIdx].second)
      {
        bestIdx = j;
      }
      else if (players[j].second == players[bestIdx].second &&
        players[j].first < players[bestIdx].first)
      {
        bestIdx = j;
      }
    }
    if (bestIdx != i)
    {
      auto temp = players[i];
      players[i] = players[bestIdx];
      players[bestIdx] = temp;
    }
  }
  std::cout << "Global Top " << n << ":\n";
  size_t limit = (n < total) ? n : total;
  for (size_t i = 0; i < limit; ++i)
  {
    std::cout << i + 1 << ". " << players[i].first << " (" << players[i].second << ")\n";
  }
}

void kuchukbaeva::Engine::loadCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  std::string filename = tokens[1];
  if (!filename.empty() && filename.back() == '\r')
  {
    filename.pop_back();
  }
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw InvalidCommandException("");
  }
  PlayerDatabase tempDb;
  HashTable< std::string, Tournament, DefaultHash< std::string >,
    EqualFunctor< std::string > > tempTours(16);

  std::string marker;
  file >> marker;
  if (marker != "[GLOBAL_DB]")
  {
    throw InvalidCommandException("");
  }

  tempDb.deserialize(file);
  std::string token;
  while (file >> token)
  {
    if (token == "[TOUR]")
    {
      Tournament tempTour;
      tempTour.deserialize(file);
      tempTours.add(tempTour.getName(), tempTour);
    }
  }

  file.close();
  if (file.bad() && !file.eof())
  {
    throw std::invalid_argument("Disk error");
  }
  globalDb_.swap(tempDb);
  activeTournaments_.swap(tempTours);
  std::cout << "<Успешно загружено>\n";
}

void kuchukbaeva::Engine::deleteCmd(const Vector< std::string >& tokens)
{
  if (tokens.getSize() != 2)
  {
    throw InvalidCommandException("");
  }
  const std::string& tName = tokens[1];
  if (!activeTournaments_.has(tName))
  {
    throw InvalidCommandException("");
  }
  Tournament& t = activeTournaments_.find(tName)->second;
  if (t.isRoundActive() || t.getCurrentRound() > 0)
  {
    throw TournamentException("Нельзя удалить турнир, который уже начался или имеет активный раунд");
  }
  Tournament dummy;
  activeTournaments_.drop(tName, dummy);
  std::cout << "<Турнир удален>\n";
}
