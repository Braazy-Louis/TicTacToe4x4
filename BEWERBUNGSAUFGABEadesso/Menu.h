#pragma once
#include <string>
#include "Game.h"
#include "Scoreboard.h"
#include "AI.h"

class Menu {
private:
    Scoreboard scoreboard;
    void displayMainMenu() const;
    void displayAIDifficultyMenu() const;
    std::string getPlayerName(int playerNumber) const;
    int getWinLimit() const;
    AIDifficulty getAIDifficulty() const;
    void clearScreen() const;
    void setColor(int color) const;

public:
    Menu();
    void run();
}; 