#pragma once
#include "Item.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class WorldItem {
public:
    WorldItem(std::shared_ptr<Item> item, const sf::Vector2f& position);

    void update(float dt);  // For future: animations, floating movement, etc.
    void draw(sf::RenderWindow& window) const;

    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    std::shared_ptr<Item> getItem() const;

private:
    std::shared_ptr<Item> item;
    sf::Vector2f position;

    // For simple visual floating animation, etc., you can add members here later
};
