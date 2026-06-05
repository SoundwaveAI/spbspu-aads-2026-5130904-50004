#include <iostream>
#include <fstream>
#include <stdexcept>
#include "commands.hpp"

template< class T >
void kuchukbaeva::sortVector(Vector< T >& vec)
{
  size_t n = vec.getSize();
  if (n <= 1)
  {
    return;
  }
  for (size_t i = 0; i < n - 1; ++i)
  {
    for (size_t j = i + 1; j < n; ++j)
    {
      if (vec[j] < vec[i])
      {
        T temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
      }
    }
  }
}

void kuchukbaeva::throwInval()
{
  throw std::invalid_argument("INVALID COMMAND");
}

kuchukbaeva::Vector< std::string > kuchukbaeva::splitString(const std::string& str)
{
  Vector< std::string > res;
  std::string current = "";
  for (size_t i = 0; i < str.length(); ++i)
  {
    if (str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t')
    {
      if (!current.empty())
      {
        res.pushBack(current);
        current = "";
      }
    }
    else
    {
      current += str[i];
    }
  }
  if (!current.empty())
  {
    res.pushBack(current);
  }
  return res;
}

bool kuchukbaeva::tryParseUInt(const std::string& str, unsigned int& out)
{
  if (str.empty())
  {
    return false;
  }
  unsigned long long res = 0;
  for (size_t i = 0; i < str.length(); ++i)
  {
    if (str[i] < '0' || str[i] > '9')
    {
      return false;
    }
    res = res * 10 + (str[i] - '0');
    if (res > 4294967295ULL)
    {
      return false;
    }
  }
  out = static_cast< unsigned int >(res);
  return true;
}

kuchukbaeva::Application::Application():
  commands_(16),
  graphs_(16)
{
  commands_.add("graphs", &Application::cmdGraphs);
  commands_.add("vertexes", &Application::cmdVertexes);
  commands_.add("outbound", &Application::cmdOutbound);
  commands_.add("inbound", &Application::cmdInbound);
  commands_.add("bind", &Application::cmdBind);
  commands_.add("cut", &Application::cmdCut);
  commands_.add("create", &Application::cmdCreate);
  commands_.add("merge", &Application::cmdMerge);
  commands_.add("extract", &Application::cmdExtract);
}

void kuchukbaeva::Application::loadFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;
  while (std::getline(file, line))
  {
    Vector< std::string > tokens = splitString(line);
    if (tokens.getSize() == 0)
    {
      continue;
    }
    if (tokens.getSize() == 2)
    {
      std::string graphName = tokens[0];
      unsigned int edgeCount = 0;
      if (!tryParseUInt(tokens[1], edgeCount))
      {
        continue;
      }
      Graph g;
      for (unsigned int i = 0; i < edgeCount; ++i)
      {
        std::string edgeLine;
        if (std::getline(file, edgeLine))
        {
          Vector< std::string > edgeTokens = splitString(edgeLine);
          if (edgeTokens.getSize() >= 3)
          {
            std::string src = edgeTokens[0];
            std::string dest = edgeTokens[1];
            unsigned int weight;
            if (tryParseUInt(edgeTokens[2], weight))
            {
              g.addEdge(src, dest, weight);
            }
          }
        }
      }
      graphs_.add(graphName, g);
    }
  }
}

void kuchukbaeva::Application::run(const std::string& filename)
{
  loadFromFile(filename);
  std::string line;
  while (std::getline(std::cin, line))
  {
    processLine(line);
  }
}

void kuchukbaeva::Application::processLine(const std::string& line)
{
  Vector< std::string > tokens = splitString(line);
  if (tokens.getSize() == 0)
  {
    return;
  }
  auto it = commands_.find(tokens[0]);
  if (it != commands_.end())
  {
    try
    {
      (it->second)(this, tokens);
    }
    catch (const std::invalid_argument&)
    {
      std::cout << "<INVALID COMMAND>" << "\n";
    }
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void kuchukbaeva::Application::cmdGraphs(Application* app, const Vector< std::string >& args)
{
  if (args.getSize() != 1)
  {
    throwInval();
  }
  Vector< std::string > names;
  for (auto it = app->graphs_.cbegin(); it != app->graphs_.cend(); ++it)
  {
    names.pushBack(it->first);
  }
  if (names.getSize() == 0)
  {
    std::cout << "\n";
    return;
  }
  sortVector(names);
  for (size_t i = 0; i < names.getSize(); ++i)
  {
    std::cout << names[i] << "\n";
  }
}

void kuchukbaeva::Application::cmdVertexes(Application* app, const Vector< std::string >& args)
{
  if (args.getSize() != 2)
  {
    throwInval();
  }
  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end())
  {
    throwInval();
  }
  Vector< std::string > verts = it->second.getVertexes();
  if (verts.getSize() == 0)
  {
    std::cout << "\n";
    return;
  }
  sortVector(verts);
  for (size_t i = 0; i < verts.getSize(); ++i)
  {
    std::cout << verts[i] << "\n";
  }
}

void kuchukbaeva::Application::cmdOutbound(Application* app, const Vector< std::string >& args)
{
  if (args.getSize() != 3)
  {
    throwInval();
  }
  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end() || !it->second.hasVertex(args[2]))
  {
    throwInval();
  }
  Vector< std::pair< std::string, unsigned int > > res;
  it->second.getOutbound(args[2], res);
  if (res.getSize() > 0)
  {
    std::string current_v = res[0].first;
    std::cout << current_v << " " << res[0].second;
    for (size_t i = 1; i < res.getSize(); ++i)
    {
      if (res[i].first == current_v)
      {
        std::cout << " " << res[i].second;
      }
      else
      {
        current_v = res[i].first;
        std::cout << "\n" << current_v << " " << res[i].second;
      }
    }
    std::cout << "\n";
  }
  else
  {
    std::cout << "\n";
  }
}

void kuchukbaeva::Application::cmdInbound(Application* app, const Vector< std::string >& args)
{
  if (args.getSize() != 3)
  {
    throwInval();
  }
  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end() || !it->second.hasVertex(args[2]))
  {
    throwInval();
  }
  Vector< std::pair< std::string, unsigned int > > res;
  it->second.getInbound(args[2], res);
  if (res.getSize() > 0)
  {
    std::string current_v = res[0].first;
    std::cout << current_v << " " << res[0].second;
    for (size_t i = 1; i < res.getSize(); ++i)
    {
      if (res[i].first == current_v)
      {
        std::cout << " " << res[i].second;
      }
      else
      {
        current_v = res[i].first;
        std::cout << "\n" << current_v << " " << res[i].second;
      }
    }
    std::cout << "\n";
  }
  else
  {
    std::cout << "\n";
  }
}

void kuchukbaeva::Application::cmdBind(Application* app, const Vector< std::string >& args)
{
  unsigned int weight = 0;
  if (args.getSize() != 5 || !tryParseUInt(args[4], weight))
  {
    throwInval();
  }
  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end())
  {
    throwInval();
  }
  it->second.addEdge(args[2], args[3], weight);
}

void kuchukbaeva::Application::cmdCut(Application* app, const Vector< std::string >& args)
{
  unsigned int weight = 0;
  if (args.getSize() != 5 || !tryParseUInt(args[4], weight))
  {
    throwInval();
  }
  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end() || !it->second.cutEdge(args[2], args[3], weight))
  {
    throwInval();
  }
}

void kuchukbaeva::Application::cmdCreate(Application* app, const Vector< std::string >& args)
{
  unsigned int count = 0;
  if (args.getSize() < 3 || !tryParseUInt(args[2], count) || args.getSize() != 3 + count)
  {
    throwInval();
  }
  if (app->graphs_.has(args[1]))
  {
    throwInval();
  }
  for (unsigned int i = 0; i < count; ++i)
  {
    for (unsigned int j = i + 1; j < count; ++j)
    {
      if (args[3 + i] == args[3 + j])
      {
        throwInval();
      }
    }
  }
  Graph newG;
  for (unsigned int i = 0; i < count; ++i)
  {
    newG.addVertex(args[3 + i]);
  }
  app->graphs_.add(args[1], newG);
}

void kuchukbaeva::Application::cmdMerge(Application* app, const Vector< std::string >& args)
{
  if (args.getSize() != 4 || app->graphs_.has(args[1]))
  {
    throwInval();
  }
  auto it1 = app->graphs_.find(args[2]);
  auto it2 = app->graphs_.find(args[3]);
  if (it1 == app->graphs_.end() || it2 == app->graphs_.end())
  {
    throwInval();
  }
  app->graphs_.add(args[1], it1->second.merge(it2->second));
}

void kuchukbaeva::Application::cmdExtract(Application* app, const Vector< std::string >& args)
{
  if (args.getSize() < 4 || app->graphs_.has(args[1]))
  {
    throwInval();
  }
  auto it = app->graphs_.find(args[2]);
  unsigned int count = 0;
  if (it == app->graphs_.end() || !tryParseUInt(args[3], count) || args.getSize() != 4 + count)
  {
    throwInval();
  }
  Graph& oldG = it->second;
  Vector< std::string > vToExtract;
  for (unsigned int i = 0; i < count; ++i)
  {
    std::string v = args[4 + i];
    for (unsigned int j = i + 1; j < count; ++j)
    {
      if (v == args[4 + j])
      {
        throwInval();
      }
    }
    if (!oldG.hasVertex(v))
    {
      throwInval();
    }
    vToExtract.pushBack(v);
  }
  app->graphs_.add(args[1], oldG.extract(vToExtract));
}
