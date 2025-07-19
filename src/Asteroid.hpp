#pragma once
#include "Entity.hpp"

class Asteroid : public Entity {
public:
    Asteroid(float x, float y, float width, float height, float speed, sf::Sprite s);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    bool checkCollision(const sf::FloatRect& other) const override;
    sf::Vector2f getPosition() const override;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
};