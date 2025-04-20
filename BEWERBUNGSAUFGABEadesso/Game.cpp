#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

Game::Game(Scoreboard& sb) : scoreboard(sb), gameEnded(false), cursorRow(1), cursorCol(1),
    winLimit(0), player1Wins(0), player2Wins(0) {
    resetBoard();
}

void Game::resetBoard() {
    board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '-'));
    cursorRow = 1;
    cursorCol = 1;
}

void Game::setColor(int color) const {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Game::printScore() const {
    setColor(14); // Gelb
    std::cout << "\n=== AKTUELLER SPIELSTAND ===\n";
    std::cout << player1.getUsername() << ": " << player1Wins << " Siege\n";
    std::cout << player2.getUsername() << ": " << player2Wins << " Siege\n";
    std::cout << "Erster mit " << winLimit << " Siegen gewinnt!\n\n";
    setColor(7); // Farbe zurücksetzen
}

void Game::printBoard() const {
    system("CLS");
    std::cout << "\n";
    
	// Score anzeigen
    printScore();
    
	// Spielerinfos anzeigen
    setColor(11);
    std::cout << "Spieler 1: " << player1.getUsername() << " (X)" << std::endl;
    std::cout << "Spieler 2: " << player2.getUsername() << " (O)" << std::endl << std::endl;
    setColor(7);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << "          ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (i == cursorRow && j == cursorCol) {
                setColor(14);
                std::cout << "[" << board[i][j] << "] ";
                setColor(7);
            } else {
                std::cout << " " << board[i][j] << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n\n";
}

bool Game::checkWin(char symbol) const {
    // Zeilen und Spalten prüfen
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j <= BOARD_SIZE - 3; ++j) {
            // Horizontal prüfen
            if (board[i][j] == symbol && 
                board[i][j+1] == symbol && 
                board[i][j+2] == symbol) {
                return true;
            }
            // Vertikal prüfen
            if (board[j][i] == symbol && 
                board[j+1][i] == symbol && 
                board[j+2][i] == symbol) {
                return true;
            }
        }
    }

    // Diagonalen prüfen
    for (int i = 0; i <= BOARD_SIZE - 3; ++i) {
        for (int j = 0; j <= BOARD_SIZE - 3; ++j) {
            if (board[i][j] == symbol && 
                board[i+1][j+1] == symbol && 
                board[i+2][j+2] == symbol) {
                return true;
            }
            if (board[i][j+2] == symbol && 
                board[i+1][j+1] == symbol && 
                board[i+2][j] == symbol) {
                return true;
            }
        }
    }
    return false;
}

bool Game::isDraw() const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}

void Game::moveCursor(char direction) {
    switch (direction) {
        case 'w': if (cursorRow > 0) --cursorRow; break;
        case 's': if (cursorRow < BOARD_SIZE - 1) ++cursorRow; break;
        case 'a': if (cursorCol > 0) --cursorCol; break;
        case 'd': if (cursorCol < BOARD_SIZE - 1) ++cursorCol; break;
    }
}

void Game::setPlayerVsAI(const std::string& playerName, AIDifficulty difficulty, int wLimit) {
    mode = GameMode::PLAYER_VS_AI;
    player1 = Player(playerName, 'X');
    player2 = Player("AI (" + AI::getDifficultyString(difficulty) + ")", 'O');
    ai = std::make_unique<AI>(difficulty);
    winLimit = wLimit;
    player1Wins = 0;
    player2Wins = 0;
    scoreboard.addPlayer(player1);
}

void Game::setPlayers(const std::string& p1Name, const std::string& p2Name, int wLimit) {
    mode = GameMode::PLAYER_VS_PLAYER;
    player1 = Player(p1Name, 'X');
    player2 = Player(p2Name, 'O');
    ai.reset();
    winLimit = wLimit;
    player1Wins = 0;
    player2Wins = 0;
    scoreboard.addPlayer(player1);
    scoreboard.addPlayer(player2);
}

void Game::handleAIMove() {
    auto [row, col] = ai->makeMove(board, player2.getSymbol());
    if (row >= 0 && col >= 0) {
        board[row][col] = player2.getSymbol();
    }
}

void Game::play() {
    bool matchEnded = false;
    
    while (!matchEnded) {
        Player* currentPlayer = &player1;
        gameEnded = false;
        resetBoard();
        
        while (!gameEnded) {
            printBoard();
            
            if (mode == GameMode::PLAYER_VS_AI && currentPlayer == &player2) {
                setColor(11);
                std::cout << "KI denkt nach..." << std::endl;
                setColor(7);
                handleAIMove();
            } else {
                setColor(11);
                std::cout << "Spieler " << currentPlayer->getUsername() 
                          << " (" << currentPlayer->getSymbol() 
                          << "), benutzen Sie WASD zum Bewegen und Enter zum Setzen Ihres Symbols." << std::endl;
                setColor(7);
                
                char move = _getch();
                
                if (move == '\r') {
                    if (board[cursorRow][cursorCol] == '-') {
                        board[cursorRow][cursorCol] = currentPlayer->getSymbol();
                    } else {
                        continue;
                    }
                } else {
                    moveCursor(move);
                    continue;
                }
            }
            
            if (checkWin(currentPlayer->getSymbol())) {
                printBoard();
                if (currentPlayer == &player1) {
                    player1Wins++;
                } else {
                    player2Wins++;
                }
                
                std::cout << "Spieler " << currentPlayer->getUsername() << " gewinnt diese Runde!" << std::endl;
                
                if (player1Wins >= winLimit || player2Wins >= winLimit) {
                    std::cout << "\nGLUECKWUNSCH! " << currentPlayer->getUsername() 
                              << " gewinnt das Spiel!" << std::endl;
                    if (mode == GameMode::PLAYER_VS_PLAYER || currentPlayer == &player1) {
                        scoreboard.updateScore(currentPlayer->getUsername());
                    }
                    matchEnded = true;
                } else {
                    std::cout << "\nDruecken Sie eine beliebige Taste für die naechste Runde...";
                    _getch();
                }
                gameEnded = true;
            } else if (isDraw()) {
                printBoard();
                std::cout << "Diese Runde endet unentschieden!" << std::endl;
                std::cout << "\nDruecken Sie eine beliebige Taste für die naechste Runde...";
                _getch();
                gameEnded = true;
            } else {
                currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
            }
        }
    }
} 