// src/scenes/Scene.hpp
#pragma once
#include <SFML/Graphics.hpp>

class Scene {
public:
    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual bool isFinished() const { return false; }
    virtual std::string nextScene() const { return ""; }
};
