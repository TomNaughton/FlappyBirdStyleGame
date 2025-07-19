#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Entity {
public:
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool checkCollision(const sf::FloatRect& other) const = 0;
    virtual sf::Vector2f getPosition() const = 0;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
};