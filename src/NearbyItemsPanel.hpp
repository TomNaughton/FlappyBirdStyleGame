#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Item.hpp"

class NearbyItemsPanel {
public:
    explicit NearbyItemsPanel(int slotSize);

    void draw(sf::RenderWindow& window);
    void update(float dt, const sf::Vector2f& mousePos, bool mousePressed, bool mouseReleased);

    void setItems(const std::vector<std::shared_ptr<Item>>& items);

    // Try to start dragging an item from the panel
    std::shared_ptr<Item> startDrag(const sf::Vector2f& mousePos);

    // Try to drop item back into panel area
    bool tryDrop(std::shared_ptr<Item> item, const sf::Vector2f& mousePos);

private:
    int slotSize;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<sf::RectangleShape> slots;

    std::shared_ptr<Item> draggedItem = nullptr;
    sf::Vector2f draggedOffset;

    sf::FloatRect panelBounds;
};
