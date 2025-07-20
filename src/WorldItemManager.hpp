#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Item.hpp"

class Player;

class WorldItemManager {
public:
    WorldItemManager();

    void update(float dt, Player& player);
    void draw(sf::RenderWindow& window) const;

    void spawnItem(std::shared_ptr<Item> item);

    std::vector<std::shared_ptr<Item>> getItemsNearPosition(const sf::Vector2f& pos, float radius) const;

    void removeItem(std::shared_ptr<Item> item);

private:
    std::vector<std::shared_ptr<Item>> items;

    const float pickupRadius = 80.f;
};
