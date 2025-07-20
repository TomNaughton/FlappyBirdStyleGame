#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Asteroid.hpp"
#include "WorldItemManager.hpp"
#include "Projectile.hpp"

class SpaceObjectManager {
public:
    SpaceObjectManager(WorldItemManager& worldItemManager);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void addAsteroid(const Asteroid& asteroid);

    std::vector<std::shared_ptr<Item>> getItemsNearPosition(const sf::Vector2f& pos, float radius) const;
    void removeItem(std::shared_ptr<Item> item);
    void removeDestroyedAsteroids();
    std::vector<Asteroid> getAsteroids() const;
    std::vector<Projectile>& getProjectiles();

private:
    std::vector<Asteroid> asteroids;

    std::vector<Projectile> projectiles;

    WorldItemManager& worldItems;

    sf::Texture asteroidDebrisTexture;
};
