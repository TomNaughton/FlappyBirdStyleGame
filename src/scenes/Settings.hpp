#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

class Settings : public Scene {
public:
    Settings();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isFinished() const override;
    std::string nextScene() const override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text backText;

    bool finished = false;
};
