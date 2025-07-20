#include "WorldItemManager.hpp"
#include "Player.hpp"
#include "iostream"
#include <cmath>

WorldItemManager::WorldItemManager() {}

void WorldItemManager::update(float dt, Player& player) {
    // Items float in space, no special movement for now
}

void WorldItemManager::draw(sf::RenderWindow& window) const {
    for (auto& item : items) {
        item->draw(window);
        // std::cout << "Drawing items" << std::endl;
    }
}

void WorldItemManager::spawnItem(std::shared_ptr<Item> item) {
    items.push_back(item);
}

std::vector<std::shared_ptr<Item>> WorldItemManager::getItemsNearPosition(const sf::Vector2f& pos, float radius) const {
    std::vector<std::shared_ptr<Item>> nearby;
    float radiusSquared = radius * radius;

    for (auto& item : items) {
        sf::Vector2f diff = item->getBounds().getPosition() - pos;
        float distSquared = diff.x * diff.x + diff.y * diff.y;
        if (distSquared <= radiusSquared) {
            std::cout << "Item found nearby" << std::endl;
            nearby.push_back(item);
        }
    }
    return nearby;
}

void WorldItemManager::removeItem(std::shared_ptr<Item> item) {
    items.erase(std::remove(items.begin(), items.end(), item), items.end());
            std::cout << "Item removed" << std::endl;
}
