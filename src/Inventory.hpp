#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "InventorySlot.hpp"
#include "Item.hpp"

class Inventory {
public:
    Inventory(int width, int height, int slotSize);

    void draw(sf::RenderWindow& window);

    void addItem(std::shared_ptr<Item> item);
    bool isVisible() const;
    void toggleVisibility() { visible = !visible; }

    void update(float dt, const sf::Vector2f& mousePos, bool mouseDown, bool mouseReleased);

    void setNearbyItems(const std::vector<std::shared_ptr<Item>>& nearby);

private:
    void drawItems(sf::RenderWindow& window);

    std::vector<InventorySlot> slots;
    bool visible = false;

    std::vector<std::shared_ptr<Item>> nearbyItems;

    std::shared_ptr<Item> draggedItem = nullptr;
    sf::Vector2f draggedOffset;
    bool isDragging = false;
};
