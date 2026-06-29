#include "playerBase.hpp"
#include <stdexcept>
#include <ostream>
#include <istream>
#include <string>

kuchukbaeva::PlayerDatabase::PlayerDatabase():
  db_(16)
{}

kuchukbaeva::PlayerDatabase::PlayerDatabase(const PlayerDatabase& other):
  db_(other.db_)
{}

kuchukbaeva::PlayerDatabase::PlayerDatabase(PlayerDatabase&& other) noexcept:
  db_(std::move(other.db_))
{}

kuchukbaeva::PlayerDatabase& kuchukbaeva::PlayerDatabase::operator=(const PlayerDatabase& other)
{
  if (this != std::addressof(other))
  {
    PlayerDatabase temp(other);
    swap(temp);
  }
  return *this;
}

kuchukbaeva::PlayerDatabase& kuchukbaeva::PlayerDatabase::operator=(PlayerDatabase&& other) noexcept
{
  if (this != std::addressof(other))
  {
    swap(other);
  }
  return *this;
}

void kuchukbaeva::PlayerDatabase::swap(PlayerDatabase& other) noexcept
{
  db_.swap(other.db_);
}

void kuchukbaeva::PlayerDatabase::addOrUpdatePlayer(const std::string& name, const PlayerStats& stats)
{
  PlayerDatabase temp(*this);
  temp.db_.add(name, stats);
  swap(temp);
}

bool kuchukbaeva::PlayerDatabase::hasPlayer(const std::string& name) const
{
  return db_.has(name);
}

kuchukbaeva::PlayerStats kuchukbaeva::PlayerDatabase::getPlayerStats(const std::string& name) const
{
  auto it = db_.find(name);
  if (it == db_.end())
  {
    throw std::invalid_argument("Player not found in database");
  }
  return it->second;
}

void kuchukbaeva::PlayerDatabase::serialize(std::ostream& os) const
{
  for (auto it = db_.cbegin(); it != db_.cend(); ++it)
  {
    os << it->first << " "
       << it->second.rating << " "
       << it->second.games << " "
       << it->second.wins << " "
       << it->second.losses << " "
       << it->second.draws << "\n";
  }
}

bool kuchukbaeva::PlayerDatabase::isEmpty() const noexcept
{
  return db_.getSize() == 0;
}

kuchukbaeva::PlayerDatabase::ConstIterator kuchukbaeva::PlayerDatabase::cbegin() const noexcept
{
  return db_.cbegin();
}

kuchukbaeva::PlayerDatabase::ConstIterator kuchukbaeva::PlayerDatabase::cend() const noexcept
{
  return db_.cend();
}

void kuchukbaeva::PlayerDatabase::deserialize(std::istream& is)
{
  std::string surname;
  int r;
  size_t g, w, l, d;
  while (is >> surname)
  {
    if (surname == "[TOURNAMENTS]")
    {
      break;
    }
    is >> r >> g >> w >> l >> d;
    addOrUpdatePlayer(surname, {r, g, w, l, d});
  }
}
