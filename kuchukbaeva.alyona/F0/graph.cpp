#include "graph.hpp"
#include <stdexcept>
#include <ostream>
#include <istream>
#include <string>

using EdgeCiter = kuchukbaeva::HTCiter< std::pair< std::string, std::string >, float, kuchukbaeva::EdgeHash, kuchukbaeva::EdgeEqual >;

kuchukbaeva::Graph::Graph():
  vertexes_(),
  edges_(DEF_CAPACITY)
{}

kuchukbaeva::Graph::Graph(const Graph& other):
  vertexes_(other.vertexes_),
  edges_(other.edges_)
{}

kuchukbaeva::Graph::Graph(Graph&& other) noexcept:
  vertexes_(std::move(other.vertexes_)),
  edges_(std::move(other.edges_))
{}

kuchukbaeva::Graph& kuchukbaeva::Graph::operator=(const Graph& other)
{
  if (this != std::addressof(other))
  {
    Graph temp(other);
    swap(temp);
  }
  return *this;
}

kuchukbaeva::Graph& kuchukbaeva::Graph::operator=(Graph&& other) noexcept
{
  if (this != std::addressof(other))
  {
    swap(other);
  }
  return *this;
}

void kuchukbaeva::Graph::swap(Graph& other) noexcept
{
  vertexes_.swap(other.vertexes_);
  edges_.swap(other.edges_);
}

void kuchukbaeva::Graph::addPlayer(const std::string& name)
{
  if (hasPlayer(name))
  {
    return;
  }

  Vector< std::string > temp = vertexes_;
  size_t left = 0;
  size_t right = temp.getSize();
  while (left < right)
  {
    size_t mid = left + (right - left) / 2;
    if (temp[mid] < name)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  temp.insert(left, name);
  vertexes_.swap(temp);
}

bool kuchukbaeva::Graph::hasPlayer(const std::string& name) const
{
  size_t left = 0;
  size_t right = vertexes_.getSize();
  while (left < right)
  {
    const size_t mid = left + (right - left) / 2;
    if (vertexes_[mid] == name)
    {
      return true;
    }
    else if (vertexes_[mid] < name)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  return false;
}

void kuchukbaeva::Graph::removePlayer(const std::string& name)
{
  size_t left = 0;
  size_t right = vertexes_.getSize();
  bool found = false;
  size_t targetIdx = 0;

  while (left < right)
  {
    const size_t mid = left + (right - left) / 2;
    if (vertexes_[mid] == name)
    {
      found = true;
      targetIdx = mid;
      break;
    }
    else if (vertexes_[mid] < name)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  if (!found)
  {
    return;
  }
  Graph temp(*this);
  temp.vertexes_.erase(targetIdx);
  size_t edgeCount = temp.edges_.getSize();
  size_t newCap = (edgeCount > 0) ? (edgeCount * 2) : 16;
  HashTable< std::pair< std::string, std::string >, float, EdgeHash, EdgeEqual > newEdges(newCap);
  for (EdgeCiter it = temp.edges_.cbegin(); it != temp.edges_.cend(); ++it)
  {
    if (it->first.first != name && it->first.second != name)
    {
      newEdges.add(it->first, it->second);
    }
  }
  temp.edges_ = std::move(newEdges);
  swap(temp);
}

void kuchukbaeva::Graph::substitutePlayer(const std::string& oldName, const std::string& newName)
{
  if (oldName == newName)
  {
    return;
  }
  if (!hasPlayer(oldName))
  {
    throw std::invalid_argument("Player to substitute does not exist");
  }
  if (hasPlayer(newName))
  {
    throw std::invalid_argument("New player name already exists");
  }
  Graph temp(*this);
  size_t left = 0;
  size_t right = temp.vertexes_.getSize();
  size_t oldIdx = 0;

  while (left < right)
  {
    const size_t mid = left + (right - left) / 2;
    if (temp.vertexes_[mid] == oldName)
    {
      oldIdx = mid;
      break;
    }
    else if (temp.vertexes_[mid] < oldName)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  temp.vertexes_.erase(oldIdx);
  left = 0;
  right = temp.vertexes_.getSize();
  while (left < right)
  {
    const size_t mid = left + (right - left) / 2;
    if (temp.vertexes_[mid] < newName)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  temp.vertexes_.insert(left, newName);
  size_t edgeCount = temp.edges_.getSize();
  size_t newCap = (edgeCount > 0) ? (edgeCount * 2) : 16;
  HashTable< std::pair< std::string, std::string >, float, EdgeHash, EdgeEqual > newEdges(newCap);
  for (EdgeCiter it = temp.edges_.cbegin(); it != temp.edges_.cend(); ++it)
  {
    std::pair< std::string, std::string > key = it->first;
    if (key.first == oldName)
    {
      key.first = newName;
    }
    if (key.second == oldName)
    {
      key.second = newName;
    }
    newEdges.add(key, it->second);
  }
  temp.edges_ = std::move(newEdges);
  swap(temp);
}

const kuchukbaeva::Vector< std::string >& kuchukbaeva::Graph::getPlayers() const noexcept
{
  return vertexes_;
}

void kuchukbaeva::Graph::addMatch(const std::string& p1, const std::string& p2, float result)
{
  if (!hasPlayer(p1) || !hasPlayer(p2))
  {
    throw std::invalid_argument("One or both players do not exist in the tournament");
  }
  Graph temp(*this);
  temp.edges_.add(std::make_pair(p1, p2), result);
  temp.edges_.add(std::make_pair(p2, p1), 1.0f - result);
  swap(temp);
}

bool kuchukbaeva::Graph::hasPlayed(const std::string& p1, const std::string& p2) const
{
  return edges_.has(std::make_pair(p1, p2));
}

float kuchukbaeva::Graph::getPlayerScore(const std::string& name) const
{
  float totalScore = 0.0f;
  for (EdgeCiter it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->first.first == name)
    {
      totalScore += it->second;
    }
  }
  return totalScore;
}

void kuchukbaeva::Graph::serialize(std::ostream& os) const
{
  for (size_t i = 0; i < vertexes_.getSize(); ++i)
  {
    os << vertexes_[i] << "\n";
  }
  os << "---\n";
  for (EdgeCiter it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    os << it->first.first << " " << it->first.second << " " << it->second << "\n";
  }
}

kuchukbaeva::Vector< std::string > kuchukbaeva::Graph::getOpponents(const std::string& name) const
{
  Vector< std::string > opponents;
  for (EdgeCiter it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->first.first == name && it->first.second != "плюсов")
    {
      opponents.pushBack(it->first.second);
    }
  }
  return opponents;
}

void kuchukbaeva::Graph::deserialize(std::istream& is)
{
  std::string token;
  while (is >> token)
  {
    if (token == "---")
    {
      break;
    }
    if (!hasPlayer(token))
    {
      addPlayer(token);
    }
  }
  std::string p1, p2;
  float res;
  while (is >> p1)
  {
    if (p1 == "---" || p1 == "===" || p1 == "[TOUR]")
    {
      break;
    }
    is >> p2 >> res;
    if (hasPlayer(p1) && hasPlayer(p2))
    {
      addMatch(p1, p2, res);
    }
  }
}
