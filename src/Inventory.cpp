#include "Inventory.hpp"

Inventory::Inventory(int width, int height, int slotSize) {
    // Create grid of slots
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int px = x * slotSize;
            int py = y * slotSize;
            slots.emplace_back(px, py, slotSize, slotSize);
        }
    }
}

void Inventory::draw(sf::RenderWindow& window) {
    // Draw slots
    for (const auto& slot : slots) {
        slot.draw(window);
    }

    // Draw items inside slots
    drawItems(window);

    // Optional: Draw nearby items panel (not implemented here)
}

void Inventory::drawItems(sf::RenderWindow& window) {
    for (const auto& slot : slots) {
        if (slot.isOccupied()) {
            slot.getItem()->draw(window);
        }
    }
}

void Inventory::addItem(std::shared_ptr<Item> item) {
    for (auto& slot : slots) {
        if (!slot.isOccupied()) {
            slot.setItem(item);
            return;
        }
    }
    // If full, could handle differently (e.g., drop item)
}

bool Inventory::isVisible() const {
    return visible;
}

void Inventory::setNearbyItems(const std::vector<std::shared_ptr<Item>>& nearby) {
    nearbyItems = nearby;
}

void Inventory::update(float dt, const sf::Vector2f& mousePos, bool mouseDown, bool mouseReleased) {
    // Handle input, drag-drop, slot selection, stacking, etc.
    // This example leaves it empty for you to implement interaction logic
}
