#pragma once
#include <vector>
#include <string>
#include "Player.h"

class Scoreboard {
private:
    std::vector<Player> players;
    const std::string SCOREBOARD_FILE = "scoreboard.txt"; // appdata wäre besser 
    bool playerExists(const std::string& username) const;

public:
    Scoreboard();
    void addPlayer(const Player& player);
    void updateScore(const std::string& username);
    void displayScoreboard() const;
    void saveToFile() const;
    void loadFromFile();
}; 