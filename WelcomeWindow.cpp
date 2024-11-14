#include "WelcomeWindow.h"
#include <cctype>

WelcomeWindow::WelcomeWindow(int& NUM_COLUMNS, int& NUM_ROWS) {
    this->HEIGHT = (NUM_ROWS * 32) + 100;
    this->WIDTH = (NUM_COLUMNS * 32);
    this->USERNAME = std::string();
}

void WelcomeWindow::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

std::string WelcomeWindow::getUsername() {
    return this->USERNAME;
}

void WelcomeWindow::rungame(bool& WELCOME_WINDOW, bool& GAME_WINDOW) {
    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Minesweeper");
    sf::Font font;

    if (!font.loadFromFile("../files/font.ttf")){
        return;
    }

    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText,float(WIDTH)/2, float(HEIGHT)/2 - 150);

    sf::Text nameText("Enter your name:", font, 20);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, float(WIDTH)/2, float(HEIGHT)/2 - 75);

    std::string input;
    sf::Text inputText("|", font, 18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, float(WIDTH)/2, float(HEIGHT)/2 - 45);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (input.size() > 0) {
                        WELCOME_WINDOW = false;
                        this->USERNAME = input;
                        GAME_WINDOW = true;
                        window.close();
                        return;
                    }
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b'){
                    if (!input.empty()){
                        input.erase(input.size() - 1, 1);
                    }
                }
                else if (isalpha(event.text.unicode) && input.size() < 10){
                    if (input.empty()){
                        input += std::toupper(event.text.unicode);
                    }
                    else {
                        input += std::tolower(event.text.unicode);
                    }
                }
                inputText.setString(input + "|");
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(nameText);
        window.draw(inputText);
        window.display();
    }
}