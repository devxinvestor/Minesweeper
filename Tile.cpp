#include <SFML/Graphics.hpp>

struct Tile {
    bool PREVIOUSLY_REVEALED;
    bool HIDDEN;
    bool MINE;
    bool FLAGGED;
    int NEIGHBORS;
    int HEIGHT;
    int WIDTH;
    int x;
    int y;
    bool PAUSED;
    sf::Sprite sprite;
    Tile(int x, int y) {
        this->HIDDEN = true;
        this->MINE = false;
        this->FLAGGED = false;
        this->NEIGHBORS = 0;
        this->x = x;
        this->y = y;
        this->PREVIOUSLY_REVEALED = false;
        this->PAUSED = false;
    };
    void drawImage(sf::RenderWindow& window){
        sf::Texture tileHidden, tileRevealed, flag, mine, number;
        sf::Sprite tileImage, flagImage, numberImage, mineImage;
        if (this->PAUSED){
            if (!tileRevealed.loadFromFile("../files/images/tile_revealed.png")){
                return;
            }
            tileImage.setTexture(tileRevealed);
            tileImage.setPosition(this->x, this->y);
            window.draw(tileImage);
        }
        else {
            if (this->HIDDEN){
                if (!tileHidden.loadFromFile("../files/images/tile_hidden.png")){
                    return;
                }
                tileImage.setTexture(tileHidden);
                tileImage.setPosition(this->x, this->y);
                window.draw(tileImage);

                if (this->FLAGGED){
                    if (!flag.loadFromFile("../files/images/flag.png")){
                        return;
                    }
                    flagImage.setTexture(flag);
                    flagImage.setPosition(this->x, this->y);
                    window.draw(flagImage);
                }
            }
            else {
                if (!tileRevealed.loadFromFile("../files/images/tile_revealed.png")){
                    return;
                }
                tileImage.setTexture(tileRevealed);
                tileImage.setPosition(this->x, this->y);
                window.draw(tileImage);

                if (this->FLAGGED){
                    if (!flag.loadFromFile("../files/images/flag.png")){
                        return;
                    }
                    flagImage.setTexture(flag);
                    flagImage.setPosition(this->x, this->y);
                    window.draw(flagImage);
                }

                if (this->MINE){
                    if (!mine.loadFromFile("../files/images/mine.png")){
                        return;
                    }
                    mineImage.setTexture(mine);
                    mineImage.setPosition(this->x, this->y);
                    window.draw(mineImage);
                }
                else {
                    if (this->NEIGHBORS > 0){
                        std::string filename = "../files/images/number_" + std::to_string(this->NEIGHBORS) + ".png";
                        if (!number.loadFromFile(filename)){
                            return;
                        }
                        numberImage.setTexture(number);
                        numberImage.setPosition(this->x, this->y);
                        window.draw(numberImage);
                    }
                }
            }
        }
    }
    void drawMine(sf::RenderWindow& window){
        sf::Texture mine;
        sf::Sprite mineImage;
        if (!mine.loadFromFile("../files/images/mine.png")){
            return;
        }
        mineImage.setTexture(mine);
        mineImage.setPosition(this->x, this->y);
        window.draw(mineImage);
    }
};