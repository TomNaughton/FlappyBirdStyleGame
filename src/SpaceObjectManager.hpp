#ifndef SPACE_OBJECT_MANAGER_HPP
#define SPACE_OBJECT_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Asteroid.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

class SpaceObjectManager {
public:
    void update(float dt, const sf::Vector2f& playerPos, float maxDistance);
    void draw(sf::RenderWindow& window);

    // Spawning methods
    void spawnAsteroid(const std::vector<sf::Vector2f>& polygon,
                       sf::Vector2f position,
                       sf::Vector2f velocity,
                       float rotationSpeed);

    void spawnProjectile(const sf::Vector2f& position,
                         float rotationDegrees,
                         float speed);

private:
    void handleCollisions();

    std::vector<Asteroid> asteroids;
    std::vector<Projectile> projectiles;
};

#endif // SPACE_OBJECT_MANAGER_HPP
