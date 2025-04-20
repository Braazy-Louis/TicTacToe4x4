#include "AI.h"
#include <random>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>

AI::AI(AIDifficulty diff) : difficulty(diff) {}

std::string AI::getDifficultyString(AIDifficulty diff) {
    switch (diff) {
        case AIDifficulty::EASY:
            return std::string("Einfach");
        case AIDifficulty::NORMAL:
            return std::string("Normal");
        case AIDifficulty::HARD:
            return std::string("Schwer");
        default:
            return std::string("Unbekannt");
    }
}

std::pair<int, int> AI::getRandomMove(const std::vector<std::vector<char>>& board) const {
    auto moves = getAvailableMoves(board);
    if (moves.empty()) return {-1, -1};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(moves.size()) - 1);
    return moves[dis(gen)];
}

bool AI::checkWinningMove(const std::vector<std::vector<char>>& board, int row, int col, char symbol) const {
    // Horizontale Prüfung
    int count = 0;
    for (int j = std::max(0, col - 2); j <= std::min(BOARD_SIZE - 1, col + 2); ++j) {
        if (board[row][j] == symbol) count++;
        else count = 0;
        if (count >= 3) return true;
    }

    // Vertikale Prüfung
    count = 0;
    for (int i = std::max(0, row - 2); i <= std::min(BOARD_SIZE - 1, row + 2); ++i) {
        if (board[i][col] == symbol) count++;
        else count = 0;
        if (count >= 3) return true;
    }

    // Diagonale Prüfung 
    count = 0;
    for (int i = -2; i <= 2; ++i) {
        int newRow = row + i;
        int newCol = col + i;
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (board[newRow][newCol] == symbol) count++;
            else count = 0;
            if (count >= 3) return true;
        }
    }

    // Diagonale Prüfung
    count = 0;
    for (int i = -2; i <= 2; ++i) {
        int newRow = row + i;
        int newCol = col - i;
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (board[newRow][newCol] == symbol) count++;
            else count = 0;
            if (count >= 3) return true;
        }
    }

    return false;
}


int AI::countConsecutive(const std::vector<std::vector<char>>& board, int row, int col, char symbol) const {
    int maxCount = 0;
    
    // Horizontale Prüfung
    int count = 0;
    for (int j = std::max(0, col - 2); j <= std::min(BOARD_SIZE - 1, col + 2); ++j) {
        if (board[row][j] == symbol) count++;
        else count = 0;
        maxCount = std::max(maxCount, count);
    }

    // Vertikale Prüfung
    count = 0;
    for (int i = std::max(0, row - 2); i <= std::min(BOARD_SIZE - 1, row + 2); ++i) {
        if (board[i][col] == symbol) count++;
        else count = 0;
        maxCount = std::max(maxCount, count);
    }

    // Diagonale Prüfung 
    count = 0;
    for (int i = -2; i <= 2; ++i) {
        int newRow = row + i;
        int newCol = col + i;
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (board[newRow][newCol] == symbol) count++;
            else count = 0;
            maxCount = std::max(maxCount, count);
        }
    }

    // Diagonale Prüfung 
    count = 0;
    for (int i = -2; i <= 2; ++i) {
        int newRow = row + i;
        int newCol = col - i;
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (board[newRow][newCol] == symbol) count++;
            else count = 0;
            maxCount = std::max(maxCount, count);
        }
    }

    return maxCount;
}

std::vector<std::pair<int, int>> AI::getAvailableMoves(const std::vector<std::vector<char>>& board) const {
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == '-') {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

int AI::evaluatePosition(const std::vector<std::vector<char>>& board, char symbol) const {
    int score = 0;
    char opponent = (symbol == 'X') ? 'O' : 'X';

    // Jede leere Position bewerten
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == '-') {
                // Prüfen, ob dieser Zug eine Gewinnlinie für die KI erzeugt
                std::vector<std::vector<char>> tempBoard = board;
                tempBoard[i][j] = symbol;
                if (checkWinningMove(tempBoard, i, j, symbol)) {
                    score -= 1000;  // Strafpunkte für Züge, die zum Verlust führen
                }
                
                // Prüfen, ob dieser Zug die Gewinnlinie des Gegners blockiert
                tempBoard[i][j] = opponent;
                if (checkWinningMove(tempBoard, i, j, opponent)) {
                    score += 800;  // Hohe Priorität für das Blockieren von Gewinnzügen des Gegners
                }
                
                // Potenzielle Zwei-in-einer-Reihe-Situationen prüfen
                tempBoard[i][j] = opponent;
                int opponentConsecutive = countConsecutive(tempBoard, i, j, opponent);
                if (opponentConsecutive == 2) {
                    score += 400;  // Priorität für das Blockieren potenzieller Zwei-in-einer-Reihe
                }
                
                // Prüfen, ob der Zug Möglichkeiten für den Gegner schafft
                tempBoard[i][j] = symbol;
                for (const auto& nextMove : getAvailableMoves(tempBoard)) {
                    tempBoard[nextMove.first][nextMove.second] = opponent;
                    if (checkWinningMove(tempBoard, nextMove.first, nextMove.second, opponent)) {
                        score -= 300;  // Strafpunkte für Züge, die dem Gegner Gewinnmöglichkeiten geben
                    }
                    tempBoard[nextMove.first][nextMove.second] = '-';
                }
            }
        }
    }

    return score;
}

std::pair<int, int> AI::getBestMove(const std::vector<std::vector<char>>& board, char aiSymbol) const {
    auto moves = getAvailableMoves(board);
    if (moves.empty()) return {-1, -1};

    std::vector<int> scores;
    for (const auto& move : moves) {
        std::vector<std::vector<char>> tempBoard = board;
        tempBoard[move.first][move.second] = aiSymbol;
        scores.push_back(evaluatePosition(tempBoard, aiSymbol));
    }

    // Zug mit der höchsten Bewertung finden
    auto maxScoreIt = std::max_element(scores.begin(), scores.end());
    int bestMoveIndex = std::distance(scores.begin(), maxScoreIt);
    return moves[bestMoveIndex];
}

std::pair<int, int> AI::makeMove(const std::vector<std::vector<char>>& board, char aiSymbol) {
    // Verzögerung hinzufügen, damit es so aussieht, als würde die KI "nachdenken" (kann entfernt werden)
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Basierend auf der Schwierigkeit unterschiedliche Strategien anwenden
    if (difficulty == AIDifficulty::EASY) {
        return getRandomMove(board);
    }
    else if (difficulty == AIDifficulty::NORMAL) {
        // 70% Chance für den besten Zug, 30% für einen zufälligen Zug
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        
        if (dis(gen) < 0.7) {
            return getBestMove(board, aiSymbol);
        }
        return getRandomMove(board);
    }
    else { // SCHWER
        return getBestMove(board, aiSymbol);
    }
} 