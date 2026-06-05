#include "commands.hpp"
#include <fstream>
#include <stdexcept>
#include <limits>
#include <iostream>

kuchukbaeva::Cmd::Cmd()
{
  commands_.push("print", &Cmd::cmdPrint);
  commands_.push("complement", &Cmd::cmdComplement);
  commands_.push("intersect", &Cmd::cmdIntersect);
  commands_.push("union", &Cmd::cmdUnion);
}

void kuchukbaeva::Cmd::loadFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string datasetName;
  while (file >> datasetName)
  {
    SingleDataset dataset;
    int key;
    std::string value;

    while (file.peek() == ' ')
    {
      file.get();
    }
    while (file.peek() != '\n' && file.peek() != EOF)
    {
      if (!(file >> key >> value)) {
        break;
      }
      dataset.push(key, value);
      while (file.peek() == ' ')
      {
        file.get();
      }
    }
    datasets_.push(datasetName, dataset);
  }
}

void kuchukbaeva::Cmd::processCmd(std::istream& in, std::ostream& out)
{
  std::string command;
  while (in >> command)
  {
    auto it = commands_.find(command);
    if (it == commands_.end())
    {
      out << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    FuncType func = it->second;
    (this->*func)(in, out);
  }
}

void kuchukbaeva::Cmd::cmdPrint(std::istream& in, std::ostream& out)
{
  std::string datasetName;
  if (!(in >> datasetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto it = datasets_.find(datasetName);
  if (it == datasets_.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const auto& dataset = it->second;
  if (dataset.isEmpty())
  {
    out << "<EMPTY>\n";
    return;
  }

  out << datasetName;
  for (auto cit = dataset.cbegin(); cit != dataset.cend(); ++cit)
  {
    out << " " << cit->first << " " << cit->second;
  }
  out << "\n";
}

void kuchukbaeva::Cmd::cmdComplement(std::istream& in, std::ostream& out)
{
  std::string newName, leftName, rightName;
  if (!(in >> newName >> leftName >> rightName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto leftIt = datasets_.find(leftName);
  auto rightIt = datasets_.find(rightName);
  if (leftIt == datasets_.end() || rightIt == datasets_.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const auto& left = leftIt->second;
  const auto& right = rightIt->second;
  SingleDataset result;

  for (auto cit = left.cbegin(); cit != left.cend(); ++cit)
  {
    if (right.find(cit->first) == right.cend())
    {
      result.push(cit->first, cit->second);
    }
  }
  datasets_.push(newName, result);
}

void kuchukbaeva::Cmd::cmdIntersect(std::istream& in, std::ostream& out)
{
  std::string newName, leftName, rightName;
  if (!(in >> newName >> leftName >> rightName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto leftIt = datasets_.find(leftName);
  auto rightIt = datasets_.find(rightName);
  if (leftIt == datasets_.end() || rightIt == datasets_.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const auto& left = leftIt->second;
  const auto& right = rightIt->second;
  SingleDataset result;

  for (auto cit = left.cbegin(); cit != left.cend(); ++cit)
  {
    if (right.find(cit->first) != right.cend())
    {
      result.push(cit->first, cit->second);
    }
  }
  datasets_.push(newName, result);
}

void kuchukbaeva::Cmd::cmdUnion(std::istream& in, std::ostream& out)
{
  std::string newName, leftName, rightName;
  if (!(in >> newName >> leftName >> rightName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto leftIt = datasets_.find(leftName);
  auto rightIt = datasets_.find(rightName);
  if (leftIt == datasets_.end() || rightIt == datasets_.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const auto& left = leftIt->second;
  const auto& right = rightIt->second;
  SingleDataset result;

  for (auto cit = left.cbegin(); cit != left.cend(); ++cit)
  {
    result.push(cit->first, cit->second);
  }
  for (auto cit = right.cbegin(); cit != right.cend(); ++cit)
  {
    if (left.find(cit->first) == left.cend())
    {
      result.push(cit->first, cit->second);
    }
  }
  datasets_.push(newName, result);
}
