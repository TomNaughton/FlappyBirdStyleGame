#include "InventorySlot.hpp"
#include <iostream>

InventorySlot::InventorySlot(int x, int y, int width, int height) {
    slotRect.setPosition(static_cast<float>(x), static_cast<float>(y));
    slotRect.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    slotRect.setFillColor(sf::Color(50, 50, 50));
    slotRect.setOutlineColor(sf::Color::White);
    slotRect.setOutlineThickness(1.f);
}

void InventorySlot::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    sf::RectangleShape tempRect = slotRect;
    tempRect.setPosition(tempRect.getPosition() + offset);
    window.draw(tempRect);

    if (item) {
        item->draw(window);
    }
}


bool InventorySlot::isOccupied() const {
    return item != nullptr;
}

std::shared_ptr<Item> InventorySlot::getItem() const {
    return item;
}

sf::FloatRect InventorySlot::getBounds() const {
    return slotRect.getGlobalBounds();
}

void InventorySlot::setItem(std::shared_ptr<Item> newItem, const sf::Vector2f& offset) {
    item = newItem;
    if (item) {
        // Local position of the slot in inventory grid
        sf::Vector2f localPos = slotRect.getPosition();  // e.g., (0,0), (64,0), etc.

        // Apply inventory offset to get screen/world position
        sf::Vector2f visualTopLeft = localPos + offset;

        // Get size of the slot
        sf::Vector2f size = slotRect.getSize();

        // Center of the slot (in screen/world space)
        sf::Vector2f center = visualTopLeft + (size / 2.f);

        std::cout << "Setting item position to: " << center.x << ", " << center.y << std::endl;
        std::cout << "Visual Top Left: " << visualTopLeft.x << ", " << visualTopLeft.y << std::endl;
        std::cout << "Slot Size: " << size.x << ", " << size.y << std::endl;

        // Place item at center
        item->setPosition(center);
    }
}

void InventorySlot::clearItem() {
    item = nullptr;
}

sf::Vector2f InventorySlot::getPosition() const {
    return slotRect.getPosition();
}