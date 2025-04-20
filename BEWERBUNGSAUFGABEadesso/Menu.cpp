#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <limits>

Menu::Menu() {}

void Menu::setColor(int color) const {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Menu::clearScreen() const {
    system("CLS");
}

void Menu::displayMainMenu() const {
    clearScreen();
    setColor(14); // Gelb
    std::cout << "\n\n";
    std::cout << "          +------------------------+\n";
    std::cout << "          | Adesso (Louis Stranz)  |\n";
    std::cout << "          +------------------------+\n\n";
    
    setColor(11); // Cyan
    std::cout << "          1. Spieler gegen Spieler\n";
    std::cout << "          2. Spieler gegen KI\n";
    std::cout << "          3. Bestenliste anzeigen\n";
    std::cout << "          4. Beenden\n\n";
    
    setColor(7); // Weiß
    std::cout << "          Wählen Sie eine Option (1-4): ";
}

void Menu::displayAIDifficultyMenu() const {
    clearScreen();
    setColor(14); // Gelb
    std::cout << "\n\n";
    std::cout << "          +------------------------+\n";
    std::cout << "          |    KI-SCHWIERIGKEIT    |\n";
    std::cout << "          +------------------------+\n\n";
    
    setColor(11); // Cyan
    std::cout << "          1. Einfach\n";
    std::cout << "          2. Normal\n";
    std::cout << "          3. Schwer\n\n";
    
    setColor(7); // Weiß
    std::cout << "          Wählen Sie eine Option (1-3): ";
}

AIDifficulty Menu::getAIDifficulty() const {
    displayAIDifficultyMenu();
    while (true) {
        char choice = _getch();
        switch (choice) {
            case '1': return AIDifficulty::EASY;
            case '2': return AIDifficulty::NORMAL;
            case '3': return AIDifficulty::HARD;
            default: continue;
        }
    }
}

std::string Menu::getPlayerName(int playerNumber) const {
    clearScreen();
    std::string name;
    setColor(11);
    std::cout << "\n\n          Geben Sie den Benutzernamen fuer Spieler " << playerNumber << " ein: ";
    setColor(7);
    std::getline(std::cin, name);
    return name;
}

int Menu::getWinLimit() const {
    clearScreen();
    int limit;
    while (true) {
        setColor(11);
        std::cout << "\n\n          Geben Sie die Anzahl der Siege ein, die zum Gewinn benoetigt werden (1-10): ";
        setColor(7);
        std::string input;
        std::getline(std::cin, input);
        
        try {
            limit = std::stoi(input);
            if (limit >= 1 && limit <= 10) {
                break;
            }
        } catch (...) {
            // Ungültige Eingabe, Schleife fortsetzen
        }
        std::cout << "\n          Bitte geben Sie eine gueltige Zahl zwischen 1 und 10 ein.";
    }
    return limit;
}

void Menu::run() {
    char choice;
    bool running = true;

    while (running) {
        displayMainMenu();
        choice = _getch();

        switch (choice) {
            case '1': {
                std::string player1Name = getPlayerName(1);
                std::string player2Name = getPlayerName(2);
                int winLimit = getWinLimit();
                
                Game game(scoreboard);
                game.setPlayers(player1Name, player2Name, winLimit);
                game.play();
                
                std::cout << "\nDruecken Sie eine beliebige Taste um fortzusetzen...";
                _getch();
                break;
            }
            case '2': {
                std::string playerName = getPlayerName(1);
                AIDifficulty difficulty = getAIDifficulty();
                int winLimit = getWinLimit();
                
                Game game(scoreboard);
                game.setPlayerVsAI(playerName, difficulty, winLimit);
                game.play();
                
                std::cout << "\nDruecken Sie eine beliebige Taste um fortzusetzen...";
                _getch();
                break;
            }
            case '3': {
                clearScreen();
                scoreboard.displayScoreboard();
                std::cout << "\nDruecken Sie eine beliebige Taste um fortzusetzen...";
                _getch();
                break;
            }
            case '4':
                running = false;
                break;
        }
    }
} 