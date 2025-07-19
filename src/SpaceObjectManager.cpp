#include "SpaceObjectManager.hpp"
#include <iostream>

void SpaceObjectManager::update(float dt, const sf::Vector2f& playerPos, float maxDistance) {
    // Update asteroids
    for (auto& asteroid : asteroids) {
        asteroid.update(dt);
    }

    // Update projectiles and remove off-screen ones
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(dt);
        if (it->isOffScreen(playerPos, maxDistance)) {
            // Print position before erasing
            std::cout << "Erasing projectile at position: " << it->getPosition().x << ", " << it->getPosition().y << std::endl;
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }

    // Check collisions
    handleCollisions();
}

void SpaceObjectManager::draw(sf::RenderWindow& window) {
    for (auto& asteroid : asteroids) {
        asteroid.draw(window);
    }
    for (auto& projectile : projectiles) {
        projectile.draw(window);
    }
}

void SpaceObjectManager::spawnAsteroid(const std::vector<sf::Vector2f>& polygon,
                                       sf::Vector2f position,
                                       sf::Vector2f velocity,
                                       float rotationSpeed) {
    asteroids.emplace_back(polygon, position, velocity, rotationSpeed);
}

void SpaceObjectManager::spawnProjectile(const sf::Vector2f& position,
                                         float rotationDegrees,
                                         float speed) {
    float rad = rotationDegrees * 3.14159265f / 180.f;
    sf::Vector2f velocity(std::cos(rad) * speed, std::sin(rad) * speed);
    projectiles.emplace_back(position, velocity);
}

void SpaceObjectManager::handleCollisions() {
    for (auto pIt = projectiles.begin(); pIt != projectiles.end();) {
        bool projectileDestroyed = false;

        for (auto aIt = asteroids.begin(); aIt != asteroids.end();) {
            if (aIt->collidable && aIt->containsPoint(pIt->getPosition())) {
                std::cout << "Splitting asteroid at: " << pIt->getPosition().x << ", " << pIt->getPosition().y << "\n";

                // Split asteroid into fragments based on impact
                std::vector<Asteroid> fragments = aIt->splitAt(pIt->getPosition(), pIt->getVelocity());

                if(fragments.size() > 1)
                    for (size_t i = 1; i < fragments.size(); i++)
                    {
                        fragments[i].collidable = false;
                    }                

                std::cout << "Fragments created: " << fragments.size() << "\n";
                for (const auto& frag : fragments) {
                    std::cout << "Fragment position: " << frag.getPosition().x << ", " << frag.getPosition().y << "\n";
                    sf::Vector2f vel = frag.getVelocity();
                    std::cout << "Fragment velocity: " << vel.x << ", " << vel.y << "\n";
                }

                std::cout << "Erasing asteroid at position: " << aIt->getPosition().x << ", " << aIt->getPosition().y << std::endl;

                // Erase asteroid and get iterator to insertion point
                aIt = asteroids.erase(aIt);

                // Insert fragments at erased asteroid's position (to keep list locality)
                aIt = asteroids.insert(aIt, fragments.begin(), fragments.end());

                // Erase projectile and get next valid iterator
                pIt = projectiles.erase(pIt);

                projectileDestroyed = true;
                break;  // Exit asteroid loop, since aIt is now valid but we want to continue with next projectile
            } else {
                ++aIt;
            }
        }

        if (!projectileDestroyed) {
            ++pIt;
        }
    }
}
