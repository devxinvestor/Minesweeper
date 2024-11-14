#include "GameWindow.h"
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>
#include <fstream>

/*
 * Add a bool to make sure that the game is not in pause
 * in order for the user to be able to click on the tiles
 */

GameWindow::GameWindow(int &NUM_COLUMNS, int &NUM_ROWS, int &NUM_MINES, std::string &USERNAME) {
    this->NUM_COLUMNS = NUM_COLUMNS;
    this->NUM_ROWS = NUM_ROWS;
    this->HEIGHT = (NUM_ROWS * 32) + 100;
    this->WIDTH = (NUM_COLUMNS * 32);
    this->TILE_HEIGHT = WIDTH/NUM_COLUMNS;
    this->TILE_WIDTH = WIDTH/NUM_COLUMNS;
    this->NUM_MINES = NUM_MINES;
    this->COUNTER = NUM_MINES;
    this->GAME_LOST = false;
    this->RESET_GAME = false;
    this->GAME_WON = false;
    this->PAUSED = false;
    this->DEBUGGED = false;
    this->LEADERBOARD_SHOWN = false;
    this->TIME_ELAPSED = 0;
    this->TIME_SPAN = 0;
    this->ELAPSED_BUFFER = 0;
    this->PREVIOUSLY_PAUSED = true;
    this->NUM_HIDDEN_TILES = 0;
    this->t2 = std::chrono::high_resolution_clock::now();
    this->USERNAME = USERNAME;
    this->PLAY_PAUSE_X = (this->NUM_COLUMNS * 32) - 240;
    this->PLAY_PAUSE_Y = 32 * (this->NUM_ROWS + 0.5);
    this->DEBUG_X = (this->NUM_COLUMNS*32) - 304;
    this->DEBUG_Y = 32*(this->NUM_ROWS + 0.5);
    this->LEADERBOARD_X = (this->NUM_COLUMNS * 32) - 176;
    this->LEADERBOARD_Y = 32 * (this->NUM_ROWS + 0.5);
    this->FACE_X = ((this->NUM_COLUMNS/2.0)*32) - 32;
    this->FACE_Y = 32*(this->NUM_ROWS + 0.5);
}

void GameWindow::drawCounter(sf::RenderWindow& window) {
    sf::Texture mineCount;
    sf::Sprite negativeMine, hundredsMine, tensMine, onesMine;
    if(!mineCount.loadFromFile("../files/images/digits.png")){
        return;
    }
    int first_num = this->COUNTER / 100;
    int second_num = (this->COUNTER % 100) / 10;
    int third_num = (this->COUNTER % 10);

    hundredsMine.setTextureRect(sf::IntRect(first_num*21, 0, 21, int(mineCount.getSize().y)));
    hundredsMine.setTexture(mineCount);
    hundredsMine.setPosition(33, 32*(this->NUM_ROWS + 0.5) + 16);
    window.draw(hundredsMine);

    tensMine.setTextureRect(sf::IntRect(second_num*21, 0, 21, int(mineCount.getSize().y)));
    tensMine.setTexture(mineCount);
    tensMine.setPosition(54, 32*(this->NUM_ROWS + 0.5) + 16);
    window.draw(tensMine);

    onesMine.setTextureRect(sf::IntRect(third_num*21, 0, 21, int(mineCount.getSize().y)));
    onesMine.setTexture(mineCount);
    onesMine.setPosition(75, 32*(this->NUM_ROWS + 0.5) + 16);
    window.draw(onesMine);

    if (this->COUNTER < 0){
        negativeMine.setTextureRect(sf::IntRect(210,0,21,int(mineCount.getSize().y)));
        negativeMine.setTexture(mineCount);
        negativeMine.setPosition(12, 32*(this->NUM_ROWS + 0.5) + 16);
        window.draw(negativeMine);
    }
}

void GameWindow::revealAdjacent(Tile ***&tilemap, int i, int j) {
    if (tilemap[i][j]->HIDDEN == false) {
        return;
    }

    if (tilemap[i][j]->NEIGHBORS > 0) {
        return;
    }

    tilemap[i][j]->HIDDEN = false;
    this->NUM_HIDDEN_TILES--;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int ni = i + dx;
            int nj = j + dy;

            if (ni < 0 || ni >= this->NUM_COLUMNS || nj < 0 || nj >= this->NUM_ROWS) {
                continue;
            }

            revealAdjacent(tilemap, ni, nj);
        }
    }
}

void GameWindow::faceButton(sf::RenderWindow &window) {
    sf::Texture face_happy, face_win, face_lose;
    sf::Sprite face;

    if (this->GAME_WON){
        if (!face_win.loadFromFile("../files/images/face_win.png")){
            return;
        }
        face.setTexture(face_win);
    }
    else if (this->GAME_LOST) {
        if (!face_lose.loadFromFile("../files/images/face_lose.png")) {
            return;
        }
        face.setTexture(face_lose);
    }
    else {
        if (!face_happy.loadFromFile("../files/images/face_happy.png")) {
            return;
        }
        face.setTexture(face_happy);
    }
    face.setPosition(this->FACE_X, this->FACE_Y);
    window.draw(face);
}

void GameWindow::playPause(sf::RenderWindow &window) {
    sf::Texture pressPause, pressPlay;
    sf::Sprite play_pause;

    if (!this->PAUSED || !this->PREVIOUSLY_PAUSED){
        if (!pressPause.loadFromFile("../files/images/pause.png")){
            return;
        }
        play_pause.setTexture(pressPause);
    }
    else {
        if (!pressPlay.loadFromFile("../files/images/play.png")){
            return;
        }
        play_pause.setTexture(pressPlay);
    }
    play_pause.setPosition(this->PLAY_PAUSE_X, this->PLAY_PAUSE_Y);
    window.draw(play_pause);
}

void GameWindow::drawTimer(sf::RenderWindow &window, std::chrono::high_resolution_clock::time_point & t1) {
    sf::Texture numbers;
    sf::Sprite tenMinutes, oneMinute, tenSeconds, oneSecond;
    if (!numbers.loadFromFile("../files/images/digits.png")){
        return;
    }


    if (!this->PAUSED && !this->GAME_WON && !this->GAME_LOST){
        this->t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(this->t2 - t1);
        this->ELAPSED_BUFFER += this->TIME_SPAN;
        this->TIME_SPAN = 0;
        this->TIME_ELAPSED = floor(time_span.count()) - this->ELAPSED_BUFFER;
    }
    else {
        std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t3 - t2);
        this->TIME_SPAN = floor(time_span.count());
    }

    int tenMinutesNum = this->TIME_ELAPSED / 600;
    int oneMinuteNum = (this->TIME_ELAPSED / 60) % 10;
    int tenSecondsNum = (this->TIME_ELAPSED % 60) / 10;
    int oneSecondNum = (this->TIME_ELAPSED%10);

    tenMinutes.setTextureRect(sf::IntRect(tenMinutesNum*21, 0, 21, int(numbers.getSize().y)));
    tenMinutes.setTexture(numbers);
    tenMinutes.setPosition((this->NUM_COLUMNS * 32) - 97, 32 * (this->NUM_ROWS + 0.5) + 16);
    window.draw(tenMinutes);

    oneMinute.setTextureRect(sf::IntRect(oneMinuteNum*21, 0, 21, int(numbers.getSize().y)));
    oneMinute.setTexture(numbers);
    oneMinute.setPosition((this->NUM_COLUMNS * 32) - 76, 32 * (this->NUM_ROWS + 0.5) + 16);
    window.draw(oneMinute);

    tenSeconds.setTextureRect(sf::IntRect(tenSecondsNum*21, 0, 21, int(numbers.getSize().y)));
    tenSeconds.setTexture(numbers);
    tenSeconds.setPosition((this->NUM_COLUMNS * 32) - 54, 32 * (this->NUM_ROWS + 0.5) + 16);
    window.draw(tenSeconds);

    oneSecond.setTextureRect(sf::IntRect(oneSecondNum*21, 0, 21, int(numbers.getSize().y)));
    oneSecond.setTexture(numbers);
    oneSecond.setPosition((this->NUM_COLUMNS * 32) - 33, 32 * (this->NUM_ROWS + 0.5) + 16);
    window.draw(oneSecond);
}

void GameWindow::countNeighbors(Tile ***&tilemap, int i, int j) {
    for (int dx = -1; dx <= 1; dx++){
        for (int dy = -1; dy <= 1; dy++){
            int new_i = i + dx;
            int new_j = j + dy;
            if (new_i < 0 || new_i >= this->NUM_COLUMNS){
                continue;
            }
            if (new_j < 0 || new_j >= this->NUM_ROWS){
                continue;
            }
            if (new_i == 0 && new_j == 0){
                continue;
            }
            Tile* node = tilemap[new_i][new_j];
            if (node->MINE){
                tilemap[i][j]->NEIGHBORS++;
            }
        }
    }
}

void GameWindow::assignNeighbors(Tile ***&tilemap) {
    for (int i = 0; i < this->NUM_COLUMNS; i++) {
        for (int j = 0; j < this->NUM_ROWS; j++) {
            if (!tilemap[i][j]->MINE) {
                countNeighbors(tilemap, i, j);
            }
        }
    }
}

void GameWindow::setMines(Tile*** &tilemap) {
    srand(static_cast<unsigned>(time(0))); // Seed random number generator

    int mines_set = 0;
    while (mines_set < this->NUM_MINES) {
        int i = rand() % this->NUM_COLUMNS;
        int j = rand() % this->NUM_ROWS;

        Tile* node = tilemap[i][j];

        if (!node->MINE && !(i == 0 && j == 0)) {
            node->MINE = true;
            mines_set++;
        }
    }
    assignNeighbors(tilemap);
}

void GameWindow::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void GameWindow::drawLeaderboard(Tile*** &tilemap, sf::RenderWindow & gamewindow) {
    bool previously_paused = false;
    if (this->PAUSED){
        previously_paused = true;
    }
    if (!this->GAME_WON && !GAME_LOST){
        this->PAUSED = true;
        for (int i = 0; i < this->NUM_COLUMNS; i++) {
            for (int j = 0; j < this->NUM_ROWS; j++) {
                tilemap[i][j]->PAUSED = true;
                tilemap[i][j]->drawImage(gamewindow);
            }
        }
    }
    else {
        for (int i = 0; i < this->NUM_COLUMNS; i++) {
            for (int j = 0; j < this->NUM_ROWS; j++) {
                tilemap[i][j]->drawImage(gamewindow);
            }
        }
    }
    gamewindow.display();

    if (this->GAME_WON && this->ELAPSED_BUFFER > 0) {
        std::ofstream file("../files/leaderboard.txt", std::ios::app);
        std::string writer = convertIntToTime(this->TIME_ELAPSED) + ", " + this->USERNAME;
        file << writer << std::endl;
        file.close();
        this->ELAPSED_BUFFER = -2;
    }

    setTopPlayers();

    float width = this->NUM_COLUMNS * 16;
    float height = this->NUM_ROWS * 16 + 50;

    sf::RenderWindow window(sf::VideoMode(width,height), "Minesweeper");
    sf::Font font;

    if (!font.loadFromFile("../files/font.ttf")){
        return;
    }

    sf::Text leaderBoardText("LEADERBOARD", font, 20);
    leaderBoardText.setFillColor(sf::Color::White);
    leaderBoardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderBoardText,width/2, height/2 - 120);

    int i = 0;
    std::string allUserOnBoard;
    while (i < 5){
        if (i == 0 && this->TOP_PLAYERS[i].second == this->USERNAME){
            allUserOnBoard += std::to_string(i+1) + "." + "\t" + this->TOP_PLAYERS[i].first + "\t" + this->TOP_PLAYERS[i].second + "*" + "\n\n";
            i++;
            continue;
        }
        allUserOnBoard += std::to_string(i+1) + "." + "\t" + this->TOP_PLAYERS[i].first + "\t" + this->TOP_PLAYERS[i].second + "\n\n";
        i++;
    }
    sf::Text users(allUserOnBoard, font, 18);
    users.setFillColor(sf::Color::White);
    users.setStyle(sf::Text::Bold);
    setText(users,width/2, height/2 + 20);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->LEADERBOARD_SHOWN = false;
                for (int i = 0; i < this->NUM_COLUMNS; i++) {
                    for (int j = 0; j < this->NUM_ROWS; j++) {
                        if (this->PAUSED) {
                            tilemap[i][j]->PAUSED = false;
                        }
                    }
                }
                window.close();
                if (!previously_paused){
                    this->PREVIOUSLY_PAUSED = false;
                }
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(leaderBoardText);
        window.draw(users);
        window.display();
    }
}

int GameWindow::convertTimeToInt(std::string &time) {
    int tenMinutesNum = int(time[0]-'0') * 600;
    int oneMinuteNum = int(time[1]-'0') * 60;
    int tenSecondsNum = int(time[3]-'0') * 10;
    int oneSecondNum = int(time[4]-'0');
    return tenMinutesNum + oneMinuteNum + tenSecondsNum + oneSecondNum;
}

std::string GameWindow::convertIntToTime(int &number) {
    std::string tenMinutesNum = std::to_string(number / 600);
    std::string oneMinuteNum = std::to_string((number / 60) % 10);
    std::string tenSecondsNum = std::to_string((number % 60) / 10);
    std::string oneSecondNum = std::to_string((number % 10));
    std::string time = tenMinutesNum + oneMinuteNum + ":" + tenSecondsNum + oneSecondNum;
    return time;
}

void GameWindow::setTopPlayers() {
    std::vector<std::pair<int, std::string>> tempVector;
    this->TOP_PLAYERS.clear();

    std::ifstream file("../files/leaderboard.txt");

    // Check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    std::string line;

    std::string player;
    std::string time;
    int num_time;

    while (true){
        if(std::getline(file, line, ',')) time = line; else break;
        if(std::getline(file, line)) player = line.substr(1, line.size()); else break;
        tempVector.push_back(std::make_pair(convertTimeToInt(time), player));
    }
    std::sort(tempVector.begin(), tempVector.end());

    for (auto i = 0; i < 5; i++){
        this->TOP_PLAYERS.push_back(std::pair(convertIntToTime(tempVector[i].first), tempVector[i].second));
    }
}

void GameWindow::rungame(bool &GAME_WINDOW, bool &LEADERBOARD_WINDOW) {
    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Minesweeper");
    sf::Font font;
    sf::Texture debugger, board;
    sf::Sprite debug, leaderboard;

    if (!font.loadFromFile("../files/font.ttf")){
        return;
    }
    if (!debugger.loadFromFile("../files/images/debug.png")){
        return;
    }
    if (!board.loadFromFile("../files/images/leaderboard.png")){
        return;
    }

    Tile*** tilemap = new Tile**[NUM_COLUMNS];
    for (int i = 0; i < NUM_COLUMNS; i++) {
        tilemap[i] = new Tile*[NUM_ROWS];
        for (int j = 0; j < NUM_ROWS; j++) {
            tilemap[i][j] = new Tile(i * TILE_WIDTH, j * TILE_HEIGHT);
            this->NUM_HIDDEN_TILES++;
        }
    }
    setMines(tilemap);

    debug.setTexture(debugger);
    debug.setPosition(this->DEBUG_X, this->DEBUG_Y);
    leaderboard.setTexture(board);
    leaderboard.setPosition(this->LEADERBOARD_X, this->LEADERBOARD_Y);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    while (window.isOpen()){

        if (this->NUM_HIDDEN_TILES == this->NUM_MINES && this->COUNTER == 0){
            this->GAME_WON = true;
            this->LEADERBOARD_SHOWN = true;
            this->NUM_HIDDEN_TILES = -1;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Button::Left){
                    if ((float(position.x) >= this->FACE_X && float(position.x) <= this->FACE_X + 64) &&
                        (float(position.y) >= this->FACE_Y && float(position.y) <= this->FACE_Y + 64)) {
                        this->COUNTER = this->NUM_MINES;
                        this->GAME_LOST = false;
                        this->RESET_GAME = false;
                        this->GAME_WON = false;
                        this->PAUSED = false;
                        this->DEBUGGED = false;
                        this->LEADERBOARD_SHOWN = false;
                        this->TIME_ELAPSED = 0;
                        this->TIME_SPAN = 0;
                        this->ELAPSED_BUFFER = 0;
                        this->PREVIOUSLY_PAUSED = true;
                        this->NUM_HIDDEN_TILES = 0;

                        for (int i = 0; i < this->NUM_COLUMNS; i++) {
                            for (int j = 0; j < this->NUM_ROWS; j++) {
                                delete tilemap[i][j];
                            }
                            delete[] tilemap[i];
                        }
                        delete[] tilemap;

                        tilemap = new Tile**[NUM_COLUMNS];
                        for (int i = 0; i < NUM_COLUMNS; i++) {
                            tilemap[i] = new Tile*[NUM_ROWS];
                            for (int j = 0; j < NUM_ROWS; j++) {
                                tilemap[i][j] = new Tile(i * TILE_WIDTH, j * TILE_HEIGHT);
                                this->NUM_HIDDEN_TILES++;
                            }
                        }

                        setMines(tilemap);
                        t1 = std::chrono::high_resolution_clock::now();
                        this->t2 = std::chrono::high_resolution_clock::now();
                    }
                }
                int x_position = std::floor(position.x / this->TILE_WIDTH), y_position = std::floor(
                        position.y / this->TILE_HEIGHT);
                if (!((x_position < this->NUM_COLUMNS && x_position >= 0) &&
                      (y_position < this->NUM_ROWS && (y_position >= 0)))) {
                    if (!this->GAME_WON && !this->GAME_LOST){
                        if ((float(position.x) >= this->PLAY_PAUSE_X && float(position.x) <= this->PLAY_PAUSE_X + 64) &&
                            (float(position.y) >= this->PLAY_PAUSE_Y && float(position.y) <= this->PLAY_PAUSE_Y + 64)) {
                            this->PAUSED = !this->PAUSED;
                        }
                        else if ((float(position.x) >= this->DEBUG_X && float(position.x) <= this->DEBUG_X + 64) &&
                                 (float(position.y) >= this->DEBUG_Y && float(position.y) <= this->DEBUG_Y + 64)) {
                            this->DEBUGGED = !this->DEBUGGED;
                        }
                    }
                    if ((float(position.x) >= this->LEADERBOARD_X && float(position.x) <= this->LEADERBOARD_X + 64) &&
                             (float(position.y) >= this->LEADERBOARD_Y && float(position.y) <= this->LEADERBOARD_Y + 64)) {
                        this->LEADERBOARD_SHOWN = !this->LEADERBOARD_SHOWN;
                    }
                    // Whatever else
                    continue;
                }
                if (!this->GAME_WON){
                    if (event.mouseButton.button == sf::Mouse::Button::Right) {
                        if (tilemap[x_position][y_position]) {
                            if (tilemap[x_position][y_position]->HIDDEN && !tilemap[x_position][y_position]->FLAGGED) {
                                (tilemap[x_position][y_position]->FLAGGED = true);
                                this->COUNTER--;
                            }
                            else if (tilemap[x_position][y_position]->HIDDEN && tilemap[x_position][y_position]->FLAGGED){
                                (tilemap[x_position][y_position]->FLAGGED = false);
                                this->COUNTER++;
                            }
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Button::Left){
                        if (tilemap[x_position][y_position]) {
                            if (tilemap[x_position][y_position]->HIDDEN) {
                                (tilemap[x_position][y_position]->PREVIOUSLY_REVEALED = true);
                                if (tilemap[x_position][y_position]->NEIGHBORS == 0 && !tilemap[x_position][y_position]->MINE){
                                    revealAdjacent(tilemap, x_position, y_position);
                                }
                                else if (tilemap[x_position][y_position]->MINE){
                                    this->GAME_LOST = true;
                                }
                                else {
                                    tilemap[x_position][y_position]->HIDDEN=false;
                                    this->NUM_HIDDEN_TILES--;
                                }
                            }
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::White);
        for (int i = 0; i < this->NUM_COLUMNS; i++) {
            for (int j = 0; j < this->NUM_ROWS; j++) {
                if (this->PAUSED && this->PREVIOUSLY_PAUSED){
                    tilemap[i][j]->PAUSED = true;
                }
                else {
                    tilemap[i][j]->PAUSED = false;
                }
                tilemap[i][j]->drawImage(window);
                if (this->DEBUGGED && !this->PAUSED){
                    if (tilemap[i][j]->MINE){
                        if (tilemap[i][j]->HIDDEN){
                            tilemap[i][j]->drawMine(window);
                        }
                    }
                }
                if (this->GAME_LOST){
                    if (tilemap[i][j]->MINE){
                        if (tilemap[i][j]->HIDDEN){
                            tilemap[i][j]->HIDDEN = false;
                            tilemap[i][j]->drawMine(window);
                        }
                    }
                }
            }
        }
        drawCounter(window);
        faceButton(window);
        window.draw(debug);
        playPause(window);
        window.draw(leaderboard);
        drawTimer(window, t1);

        if (!PREVIOUSLY_PAUSED){
            this->PAUSED = false;
            this->PREVIOUSLY_PAUSED = true;
        }

        if (this->LEADERBOARD_SHOWN){
            drawLeaderboard(tilemap, window);
        }

        window.display();
    }
    for (int i = 0; i < this->NUM_COLUMNS; i++) {
        for (int j = 0; j < this->NUM_ROWS; j++) {
            delete tilemap[i][j];
        }
        delete[] tilemap[i];
    }
    delete[] tilemap;
}
