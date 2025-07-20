#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Item.hpp"

class InventorySlot {
public:
    InventorySlot(int x, int y, int width, int height);

    void draw(sf::RenderWindow& window) const;
    bool isOccupied() const;
    std::shared_ptr<Item> getItem() const;
    sf::FloatRect getBounds() const;

    void setItem(std::shared_ptr<Item> item);
    void clearItem();

private:
    sf::RectangleShape slotRect;
    std::shared_ptr<Item> item;  // nullptr if empty
};
