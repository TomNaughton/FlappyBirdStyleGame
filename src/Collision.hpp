#include "Asteroid.hpp"
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>

bool checkCollision(const Asteroid& asteroid, const Projectile& projectile) {
    sf::Vector2f diff = asteroid.getPosition() - projectile.getPosition();
    float distanceSquared = diff.x * diff.x + diff.y * diff.y;
    float combinedRadius = asteroid.getRadius() + projectile.getRadius();
    return distanceSquared < combinedRadius * combinedRadius;
}
