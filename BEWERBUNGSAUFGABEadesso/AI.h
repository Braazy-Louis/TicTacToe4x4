#pragma once
#include <vector>
#include <utility>
#include <string>

enum class AIDifficulty {
    EASY,
    NORMAL,
    HARD
};

class AI {
private:
    AIDifficulty difficulty;
    static const int BOARD_SIZE = 4;
    
    
    std::pair<int, int> getRandomMove(const std::vector<std::vector<char>>& board) const;
    std::pair<int, int> getBestMove(const std::vector<std::vector<char>>& board, char aiSymbol) const;
    int evaluatePosition(const std::vector<std::vector<char>>& board, char symbol) const;
    bool checkWinningMove(const std::vector<std::vector<char>>& board, int row, int col, char symbol) const;
    std::vector<std::pair<int, int>> getAvailableMoves(const std::vector<std::vector<char>>& board) const;
    int countConsecutive(const std::vector<std::vector<char>>& board, int row, int col, char symbol) const;

public:
    explicit AI(AIDifficulty diff);
    std::pair<int, int> makeMove(const std::vector<std::vector<char>>& board, char aiSymbol);
    static std::string getDifficultyString(AIDifficulty diff);
}; 