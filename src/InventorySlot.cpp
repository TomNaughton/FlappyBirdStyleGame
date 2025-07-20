#include "InventorySlot.hpp"

InventorySlot::InventorySlot(int x, int y, int width, int height) {
    slotRect.setPosition(static_cast<float>(x), static_cast<float>(y));
    slotRect.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    slotRect.setFillColor(sf::Color(50, 50, 50));
    slotRect.setOutlineColor(sf::Color::White);
    slotRect.setOutlineThickness(1.f);
}

void InventorySlot::draw(sf::RenderWindow& window) const {
    window.draw(slotRect);
    if (item) {
        item->draw(window);  // Assumes Item::draw positions the sprite correctly
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

void InventorySlot::setItem(std::shared_ptr<Item> newItem) {
    item = newItem;
    if (item) {
        // Position item sprite to slot center or top-left as needed
        // For example:
        sf::FloatRect bounds = slotRect.getGlobalBounds();
        item->setPosition(sf::Vector2f(bounds.left + 5.f, bounds.top + 5.f));
    }
}

void InventorySlot::clearItem() {
    item = nullptr;
}
