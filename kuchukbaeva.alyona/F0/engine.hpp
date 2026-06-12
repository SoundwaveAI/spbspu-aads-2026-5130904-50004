#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include "playerBase.hpp"
#include "tour.hpp"
#include "vec.hpp"
#include "tabRobin.hpp"

namespace kuchukbaeva
{
  class Engine
  {
  public:
    Engine();
    ~Engine() = default;
    void run();

  private:
    PlayerDatabase globalDb_;
    HashTable< std::string, Tournament > activeTournaments_;

    void processCommand(const std::string& line);
    void split(const std::string& line, Vector< std::string >& tokens) const;
    void createCmd(const Vector< std::string >& tokens);
    void mergeCmd(const Vector< std::string >& tokens);
    void joinCmd(const Vector< std::string >& tokens);
    void leaveCmd(const Vector< std::string >& tokens);
    void substituteCmd(const Vector< std::string >& tokens);
    void startRoundCmd(const Vector< std::string >& tokens);
    void matchCmd(const Vector< std::string >& tokens);
    void nextRoundCmd(const Vector< std::string >& tokens);
    void standingsCmd(const Vector< std::string >& tokens);
    void statsCmd(const Vector< std::string >& tokens);
    void saveCmd(const Vector< std::string >& tokens);
    void infoCmd(const Vector< std::string >& tokens);
    void topCmd(const Vector< std::string >& tokens);
    void loadCmd(const Vector< std::string >& tokens);
    void deleteCmd(const Vector< std::string >& tokens);
  };
}

#endif
