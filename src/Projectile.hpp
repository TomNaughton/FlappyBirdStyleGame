// Projectile.hpp
#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    bool isOffScreen(const sf::Vector2f& playerPos, float maxDistance) const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};
