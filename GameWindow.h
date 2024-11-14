#include <iostream>
#include <SFML/Graphics.hpp>
#include <cctype>
#include "Tile.cpp"
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>
#include <fstream>
#pragma once

class GameWindow {
private:
    int NUM_COLUMNS;
    int NUM_ROWS;
    int NUM_MINES;
    int HEIGHT;
    int WIDTH;
    int TILE_HEIGHT;
    int TILE_WIDTH;
    int COUNTER;
    bool GAME_LOST;
    bool DEBUGGED;
    int NUM_HIDDEN_TILES;
    bool RESET_GAME;
    bool LEADERBOARD_SHOWN;
    bool GAME_WON;
    bool PAUSED;
    int TIME_ELAPSED;
    float PLAY_PAUSE_X;
    float PLAY_PAUSE_Y;
    float DEBUG_X;
    float DEBUG_Y;
    float LEADERBOARD_X;
    float LEADERBOARD_Y;
    float FACE_X;
    float FACE_Y;
    int TIME_SPAN;
    int ELAPSED_BUFFER;
    bool PREVIOUSLY_PAUSED;
    std::vector <std::pair<std::string, std::string>> TOP_PLAYERS;
    std::string USERNAME;
    std::chrono::high_resolution_clock::time_point t2;
    void drawCounter(sf::RenderWindow& window);
    void faceButton(sf::RenderWindow& window);
    void playPause(sf::RenderWindow& window);
    void drawTimer(sf::RenderWindow& window, std::chrono::high_resolution_clock::time_point & t1);
    void setMines(Tile*** &tilemap);
    void assignNeighbors(Tile*** &tilemap);
    void countNeighbors(Tile ***&tilemap, int i, int j);
    void revealAdjacent(Tile ***&tilemap, int i, int j);
    void drawLeaderboard(Tile*** &tilemap, sf::RenderWindow & gamewindow);
    void setTopPlayers();
    void setText(sf::Text &text, float x, float y);
    int convertTimeToInt(std::string& time);
    std::string convertIntToTime(int& number);
public:
    GameWindow(int& NUM_COLUMNS, int& NUM_ROWS, int& NUM_MINES, std::string& USERNAME);
    void rungame(bool& GAME_WINDOW, bool& LEADERBOARD_WINDOW);
};