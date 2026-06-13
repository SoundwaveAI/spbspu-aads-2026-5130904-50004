#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <functional>
#include "vec.hpp"
#include "tabRobin.hpp"

namespace kuchukbaeva
{
  struct EdgeHash
  {
    std::size_t operator()(const std::pair< std::string, std::string >& p) const
    {
      std::size_t h1 = std::hash< std::string >{}(p.first);
      std::size_t h2 = std::hash< std::string >{}(p.second);
      return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
  };

  struct EdgeEqual
  {
    bool operator()(const std::pair< std::string,
      std::string >& a, const std::pair< std::string, std::string >& b) const
    {
      return a.first == b.first && a.second == b.second;
    }
  };

  class Graph
  {
  public:
    static constexpr size_t DEF_CAPACITY = 16;
    Graph();
    Graph(const Graph& other);
    Graph(Graph&& other) noexcept;
    ~Graph() = default;
    Graph& operator=(const Graph& other);
    Graph& operator=(Graph&& other) noexcept;

    void swap(Graph& other) noexcept;
    void addPlayer(const std::string& name);
    void removePlayer(const std::string& name);
    void substitutePlayer(const std::string& oldName, const std::string& newName);
    bool hasPlayer(const std::string& name) const;
    const Vector< std::string >& getPlayers() const noexcept;
    void addMatch(const std::string& p1, const std::string& p2, float result);
    bool hasPlayed(const std::string& p1, const std::string& p2) const;
    float getPlayerScore(const std::string& name) const;
    void serialize(std::ostream& os) const;
    Vector< std::string > getOpponents(const std::string& name) const;
    void deserialize(std::istream& is);

  private:
    Vector< std::string > vertexes_;
    HashTable< std::pair< std::string, std::string >, float, EdgeHash, EdgeEqual > edges_;
  };
}

#endif
