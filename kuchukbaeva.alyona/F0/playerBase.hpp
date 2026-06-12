#ifndef PLAYERBASE_HPP
#define PLAYERBASE_HPP

#include <string>
#include "tabRobin.hpp"

namespace kuchukbaeva
{
  struct PlayerStats
  {
    int rating = 0;
    size_t games = 0;
    size_t wins = 0;
    size_t losses = 0;
    size_t draws = 0;
  };

  class PlayerDatabase
  {
  public:
    using ConstIterator = HTCiter< std::string, PlayerStats, DefaultHash<std::string>, EqualFunctor<std::string> >;

    PlayerDatabase();
    PlayerDatabase(const PlayerDatabase& other);
    PlayerDatabase(PlayerDatabase&& other) noexcept;
    ~PlayerDatabase() = default;
    PlayerDatabase& operator=(const PlayerDatabase& other);
    PlayerDatabase& operator=(PlayerDatabase&& other) noexcept;
    void swap(PlayerDatabase& other) noexcept;
    void addOrUpdatePlayer(const std::string& name, const PlayerStats& stats);
    bool hasPlayer(const std::string& name) const;
    PlayerStats getPlayerStats(const std::string& name) const;
    void serialize(std::ostream& os) const;
    bool isEmpty() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;
    void deserialize(std::istream& is);

  private:
    HashTable< std::string, PlayerStats > db_;
  };
}

#endif
