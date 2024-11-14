#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "WelcomeWindow.h"
#include "GameWindow.h"

int main() {
    int NUM_COLUMNS, NUM_ROWS, NUM_MINES;
    bool WELCOME_WINDOW = true, GAME_WINDOW = false, LEADERBOARD_WINDOW = false;
    bool GAME_RUNNING = true;

    std::vector<int> values;
    std::ifstream file("../files/config.cfg");

    // Check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    std::string line;
    std::string USERNAME;

    if (std::getline(file, line)) NUM_COLUMNS = std::stoi(line); else return 1;
    if (std::getline(file, line)) NUM_ROWS = std::stoi(line); else return 1;
    if (std::getline(file, line)) NUM_MINES = std::stoi(line); else return 1;

    while (GAME_RUNNING){
        WelcomeWindow window(NUM_COLUMNS, NUM_ROWS);
        if (WELCOME_WINDOW){
            window.rungame(WELCOME_WINDOW, GAME_WINDOW);
        }
        USERNAME = window.getUsername();
        GameWindow gamewindow(NUM_COLUMNS, NUM_ROWS, NUM_MINES, USERNAME);
        if (GAME_WINDOW){
            gamewindow.rungame(GAME_WINDOW, LEADERBOARD_WINDOW);
        }
        GAME_RUNNING = false;
    }
    return 0;
}
