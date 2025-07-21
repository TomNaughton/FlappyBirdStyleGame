#pragma once
#include <SFML/Graphics.hpp>

class Asteroid {
public:
    Asteroid(const sf::Vector2f& position, float radius, const sf::Texture& texture);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    void setVelocity(const sf::Vector2f& vel);

    bool isDestroyed() const;

    void markForRemoval();
    bool isMarkedForRemoval() const;

    sf::FloatRect getBounds() const;

    bool canBeHit = true;

private:
    sf::Vector2f velocity;
    bool destroyed = false;
    bool toRemove = false;
    sf::Sprite sprite;
    const sf::Texture* texture = nullptr;
};
