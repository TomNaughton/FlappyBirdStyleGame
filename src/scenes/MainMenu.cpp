#include "MainMenu.hpp"
#include "SceneManager.hpp"

MainMenu::MainMenu() {
    font.loadFromFile("../assets/arial.ttf");

    titleText.setFont(font);
    titleText.setString("Gravitas");
    titleText.setCharacterSize(48);
    titleText.setPosition(100, 100);

    startPrompt.setFont(font);
    startPrompt.setString("Press Space to Start");
    startPrompt.setCharacterSize(24);
    startPrompt.setPosition(100, 200);
}

void MainMenu::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        startGame = true;
    }                
}

void MainMenu::update(float) {}

void MainMenu::render(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(startPrompt);
}

bool MainMenu::isFinished() const {
    return startGame;
}

std::string MainMenu::nextScene() const {
    return "InGame";
}
