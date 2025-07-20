#include "SpaceObjectManager.hpp"
#include "iostream"

SpaceObjectManager::SpaceObjectManager(WorldItemManager& worldItemManager)
    : worldItems(worldItemManager) {
        if (!asteroidDebrisTexture.loadFromFile("assets/asteroid_debris.png")) {
             std::cout << "Failed to load asteroid debris texture!!" << std::endl;
        }
    }

void SpaceObjectManager::update(float dt) {
    // Update asteroids
    for (auto& asteroid : asteroids) {
        asteroid.update(dt);
    }

    // Update projectiles
    for (auto& projectile : projectiles) {
        projectile.update(dt);
    }

    // Check collisions between projectiles and asteroids
    for (auto& projectile : projectiles) {
        for (auto& asteroid : asteroids) {
            if (projectile.getBounds().intersects(asteroid.getBounds())) {
                asteroid.markForRemoval();
                projectile.markForRemoval();

                // Spawn debris item in the world at asteroid's position
                std::shared_ptr<Item> debris = std::make_shared<Item>(
                ItemType::Resource, "Asteroid Debris", asteroidDebrisTexture);
                std::cout << "Asteroid position: " << asteroid.getPosition().x << ", " << asteroid.getPosition().y << std::endl;
                debris->setPosition(asteroid.getPosition());
                worldItems.spawnItem(debris);
                std::cout << "Asteroid debris created" << std::endl;
            }
        }
    }

    // Remove projectiles marked for removal
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return p.shouldBeRemoved(); }),
        projectiles.end());

    // Remove asteroids marked for removal
    asteroids.erase(
        std::remove_if(asteroids.begin(), asteroids.end(),
            [](const Asteroid& a) { return a.isMarkedForRemoval(); }),
        asteroids.end());
}


void SpaceObjectManager::draw(sf::RenderWindow& window) {
    for (auto& asteroid : asteroids) {
        asteroid.draw(window);
    }

    for (auto& projectile : projectiles) {
        projectile.draw(window);
    }
}

void SpaceObjectManager::addAsteroid(const Asteroid& asteroid) {
    asteroids.push_back(asteroid);
}

std::vector<std::shared_ptr<Item>> SpaceObjectManager::getItemsNearPosition(const sf::Vector2f& pos, float radius) const {
    return worldItems.getItemsNearPosition(pos, radius);
}

void SpaceObjectManager::removeItem(std::shared_ptr<Item> item) {
    worldItems.removeItem(item);
}

void SpaceObjectManager::removeDestroyedAsteroids() {
    asteroids.erase(
        std::remove_if(asteroids.begin(), asteroids.end(),
            [](const Asteroid& a) { return a.isMarkedForRemoval(); }),
        asteroids.end()
    );
}

std::vector<Asteroid> SpaceObjectManager::getAsteroids() const {
    return asteroids;
}

std::vector<Projectile>& SpaceObjectManager::getProjectiles() {
    return projectiles;
}