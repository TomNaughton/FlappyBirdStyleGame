#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
public:
    MainMenu();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isFinished() const override;
    std::string nextScene() const override;

private:
    bool startGame = false;
    sf::Font font;
    sf::Text titleText;
    sf::Text startPrompt;
};
