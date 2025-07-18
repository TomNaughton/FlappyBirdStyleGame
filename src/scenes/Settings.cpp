#include "Settings.hpp"

Settings::Settings() {
    font.loadFromFile("assets/font.ttf");

    titleText.setFont(font);
    titleText.setString("Settings");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(100, 100);

    backText.setFont(font);
    backText.setString("Press [ESC] to return to Main Menu");
    backText.setCharacterSize(24);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(100, 200);
}

void Settings::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        finished = true;
    }
}

void Settings::update(float) {
    // Add toggle options here later (e.g., sound, difficulty, etc.)
}

void Settings::render(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(backText);
}

bool Settings::isFinished() const {
    return finished;
}

std::string Settings::nextScene() const {
    return "MainMenu";
}
