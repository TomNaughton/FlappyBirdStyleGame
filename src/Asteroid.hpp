#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Asteroid {
public:
    Asteroid(const std::vector<sf::Vector2f>& polygon,
             sf::Vector2f position,
             sf::Vector2f velocity,
             float rotationSpeed);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    std::vector<Asteroid> splitAt(const sf::Vector2f& impactPoint, const sf::Vector2f& projectileVelocity);

    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;

    bool containsPoint(const sf::Vector2f& point) const;

    bool collidable = true;

private:
    std::vector<sf::Vector2f> polygon; // Local points relative to center
    sf::Vector2f position;
    sf::Vector2f velocity;
    float rotationSpeed;
    float rotation;
};
