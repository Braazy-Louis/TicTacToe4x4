#include "Scoreboard.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

Scoreboard::Scoreboard() {
    loadFromFile();
}

bool Scoreboard::playerExists(const std::string& username) const {
    return std::find_if(players.begin(), players.end(),
        [&username](const Player& p) { return p.getUsername() == username; }) != players.end();
}

void Scoreboard::addPlayer(const Player& player) {
    // Spieler nur hinzufügen, wenn er noch nicht existiert
    if (!playerExists(player.getUsername())) {
        players.push_back(player);
    }
}

void Scoreboard::updateScore(const std::string& username) {
    for (auto& player : players) {
        if (player.getUsername() == username) {
            player.incrementWins();
            saveToFile();
            return;
        }
    }
    // Wenn Spieler nicht existiert, mit einem Sieg hinzufügen
    Player newPlayer(username);
    newPlayer.incrementWins();
    players.push_back(newPlayer);
    saveToFile();
}

void Scoreboard::displayScoreboard() const {
    std::cout << "\n=== BESTENLISTE ===" << std::endl;
    std::cout << std::setw(20) << "Spieler" << std::setw(10) << "Siege" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    // Spieler nach Siegen sortieren (absteigend)
    std::vector<Player> sortedPlayers = players;
    std::sort(sortedPlayers.begin(), sortedPlayers.end(),
              [](const Player& a, const Player& b) { return a.getWins() > b.getWins(); });
    
    for (const auto& player : sortedPlayers) {
        std::cout << std::setw(20) << player.getUsername() 
                  << std::setw(10) << player.getWins() << std::endl;
    }
    std::cout << std::string(30, '-') << std::endl;
}

void Scoreboard::saveToFile() const {
    std::ofstream file(SCOREBOARD_FILE);
    if (file.is_open()) {
        for (const auto& player : players) {
            file << player.getUsername() << " " << player.getWins() << "\n";
        }
        file.close();
    }
}

void Scoreboard::loadFromFile() {
    std::ifstream file(SCOREBOARD_FILE);
    if (file.is_open()) {
        players.clear();
        std::string username;
        int wins;
        while (file >> username >> wins) {
            if (!playerExists(username)) {  // Nur laden, wenn Spieler nicht existiert
                Player player(username);
                for (int i = 0; i < wins; ++i) {
                    player.incrementWins();
                }
                players.push_back(player);
            }
        }
        file.close();
    }
} 