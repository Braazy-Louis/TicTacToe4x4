#include "Player.h"

Player::Player(const std::string& name, char sym) : username(name), wins(0), symbol(sym) {}

std::string Player::getUsername() const {
    return username;
}

int Player::getWins() const {
    return wins;
}

char Player::getSymbol() const {
    return symbol;
}

void Player::setUsername(const std::string& name) {
    username = name;
}

void Player::incrementWins() {
    wins++;
}

void Player::setSymbol(char sym) {
    symbol = sym;
} 