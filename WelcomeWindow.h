#include <iostream>
#include <SFML/Graphics.hpp>
#include <cctype>
#pragma once

class WelcomeWindow{
private:
    int HEIGHT;
    int WIDTH;
    std::string USERNAME;
    static void setText(sf::Text &text, float x, float y);
public:
    WelcomeWindow(int& NUM_COLUMNS, int& NUM_ROWS);
    void rungame(bool &WELCOME_WINDOW, bool &GAME_WINDOW);
    std::string getUsername();
};