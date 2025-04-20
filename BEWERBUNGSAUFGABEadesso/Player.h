#pragma once
#include <string>

class Player {
private:
    std::string username;
    int wins;
    char symbol;

public:
    Player(const std::string& name = "", char sym = 'X');
    

    std::string getUsername() const;
    int getWins() const;
    char getSymbol() const;
  
    void setUsername(const std::string& name);
    void incrementWins();
    void setSymbol(char sym);
}; 