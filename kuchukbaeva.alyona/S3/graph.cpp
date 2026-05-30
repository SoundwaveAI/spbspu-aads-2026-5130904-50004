#include <algorithm>
#include "graph.hpp"

kuchukbaeva::Graph::Graph():
  vertexes_(),
  edges_(16)
{}

void kuchukbaeva::Graph::addVertex(const std::string& v)
{
  size_t left = 0;
  size_t right = vertexes_.getSize();
  while (left < right)
  {
    size_t mid = left + (right - left) / 2;
    if (vertexes_[mid] < v)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }

  if (left < vertexes_.getSize() && vertexes_[left] == v)
  {
    return;
  }

  Vector< std::string > newVertexes = vertexes_;
  newVertexes.insert(left, v);
  vertexes_.swap(newVertexes);
}

bool kuchukbaeva::Graph::hasVertex(const std::string& v) const noexcept
{
  size_t left = 0;
  size_t right = vertexes_.getSize();
  while (left < right)
  {
    size_t mid = left + (right - left) / 2;
    if (vertexes_[mid] < v)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  return left < vertexes_.getSize() && vertexes_[left] == v;
}

void kuchukbaeva::Graph::addEdge(const std::string& src, const std::string& dest, unsigned int weight)
{
  Graph copy = *this;
  copy.addVertex(src);
  copy.addVertex(dest);

  std::pair< std::string, std::string > key = std::make_pair(src, dest);
  HTIter< std::pair< std::string, std::string >, Vector< unsigned int >, EdgeHash, EdgeEqual > it = copy.edges_.find(key);
  if (it != copy.edges_.end())
  {
    Vector< unsigned int > weights = it->second;
    weights.pushBack(weight);
    copy.edges_.add(key, weights);
  }
  else
  {
    Vector< unsigned int > newWeights;
    newWeights.pushBack(weight);
    copy.edges_.add(key, newWeights);
  }
  swap(copy);
}

bool kuchukbaeva::Graph::cutEdge(const std::string& src, const std::string& dest, unsigned int weight)
{
  std::pair< std::string, std::string > key = std::make_pair(src, dest);
  if (!edges_.has(key))
  {
    return false;
  }

  Graph copy = *this;
  HTIter< std::pair< std::string, std::string >, Vector< unsigned int >, EdgeHash, EdgeEqual > it = copy.edges_.find(key);
  Vector< unsigned int > weights = it->second;
  for (size_t i = 0; i < weights.getSize(); ++i)
  {
    if (weights[i] == weight)
    {
      weights.erase(i);
      if (weights.isEmpty())
      {
        copy.edges_.drop(key);
      }
      else
      {
        copy.edges_.add(key, weights);
      }
      swap(copy);
      return true;
    }
  }
  return false;
}

void kuchukbaeva::Graph::swap(Graph& other) noexcept
{
  vertexes_.swap(other.vertexes_);
  edges_.swap(other.edges_);
}

kuchukbaeva::Graph kuchukbaeva::Graph::merge(const Graph& other) const
{
  Graph res = *this;
  for (size_t i = 0; i < other.vertexes_.getSize(); ++i)
  {
    res.addVertex(other.vertexes_[i]);
  }
  for (auto it = other.edges_.cbegin(); it != other.edges_.cend(); ++it)
  {
    const std::pair< std::string, std::string >& key = it->first;
    const Vector< unsigned int >& weights = it->second;
    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      res.addEdge(key.first, key.second, weights[i]);
    }
  }
  return res;
}

kuchukbaeva::Graph kuchukbaeva::Graph::extract(const Vector< std::string >& vToExtract) const
{
  Graph res;
  for (size_t i = 0; i < vToExtract.getSize(); ++i)
  {
    res.addVertex(vToExtract[i]);
  }
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (res.hasVertex(it->first.first) && res.hasVertex(it->first.second))
    {
      const Vector< unsigned int >& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        res.addEdge(it->first.first, it->first.second, weights[i]);
      }
    }
  }
  return res;
}

const kuchukbaeva::Vector< std::string >& kuchukbaeva::Graph::getVertexes() const noexcept
{
  return vertexes_;
}

void kuchukbaeva::Graph::getOutbound(const std::string& v, Vector< std::pair< std::string, unsigned int > >& out) const
{
  Vector< std::pair< std::string, unsigned int > > temp;
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->first.first == v)
    {
      for (size_t i = 0; i < it->second.getSize(); ++i)
      {
        temp.pushBack(std::make_pair(it->first.second, it->second[i]));
      }
    }
  }
  sortPairs(temp);
  out.swap(temp);
}

void kuchukbaeva::Graph::getInbound(const std::string& v, Vector< std::pair< std::string, unsigned int > >& out) const
{
  Vector< std::pair< std::string, unsigned int > > temp;
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->first.second == v)
    {
      for (size_t i = 0; i < it->second.getSize(); ++i)
      {
        temp.pushBack(std::make_pair(it->first.first, it->second[i]));
      }
    }
  }
  sortPairs(temp);
  out.swap(temp);
}

void kuchukbaeva::Graph::sortPairs(Vector< std::pair< std::string, unsigned int > >& vec) const
{
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    for (size_t j = i + 1; j < vec.getSize(); ++j)
    {
      if (vec[j].first < vec[i].first || (vec[j].first == vec[i].first && vec[j].second < vec[i].second))
      {
        std::swap(vec[i], vec[j]);
      }
    }
  }
  return vec;
}
