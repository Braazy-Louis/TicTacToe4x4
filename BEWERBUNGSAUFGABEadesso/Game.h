#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Player.h"
#include "Scoreboard.h"
#include "AI.h"

enum class GameMode {
    PLAYER_VS_PLAYER,
    PLAYER_VS_AI
};

class Game {
private:
    static const int BOARD_SIZE = 4;
    std::vector<std::vector<char>> board;
    Player player1;
    Player player2;
    std::unique_ptr<AI> ai;
    GameMode mode;
    Scoreboard& scoreboard;
    bool gameEnded;
    int cursorRow;
    int cursorCol;
    int winLimit;
    int player1Wins;
    int player2Wins;

    // Spiellogik
    void printBoard() const;
    bool checkWin(char symbol) const;
    bool isDraw() const;
    void moveCursor(char direction);
    void setColor(int color) const;
    void resetBoard();
    void printScore() const;
    void handleAIMove();

public:
    explicit Game(Scoreboard& sb);
    void setPlayers(const std::string& p1Name, const std::string& p2Name, int wLimit);
    void setPlayerVsAI(const std::string& playerName, AIDifficulty difficulty, int wLimit);
    void play();
}; 