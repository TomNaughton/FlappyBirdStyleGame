#include "Inventory.hpp"
#include <iostream>

Inventory::Inventory(int width, int height, int slotSize): width(width), height(height), slotSize(slotSize) {
    // Create grid of slots
    std::cout << "Creating inventory grid with size: width:" << width << ", height:" << height << "\n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int px = x * slotSize;
            int py = y * slotSize;
            slots.emplace_back(px, py, slotSize, slotSize);
        }
    }
}

void Inventory::draw(sf::RenderWindow& window) const {
    if (!visible) return;
    std::cout << "Drawing inventory...\n" << std::endl;

    // Get view center and size
    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    // Inventory dimensions
    int totalWidth = width * slotSize;
    int totalHeight = height * slotSize;

    // Top-left of the centered inventory panel
    sf::Vector2f offset(
        viewCenter.x - totalWidth / 2.f,
        viewCenter.y - totalHeight / 2.f
    );

    // Draw slots
    for (const auto& slot : slots) {
        slot.draw(window, offset);  // << pass offset
    }

    // Draw dragged item
    if (isDragging && draggedItem) {
        draggedItem->draw(window);
    }
    std::cout << "Slots: " << slots.size() << "\n";

    std::cout << "Slot position: " << slots[0].getPosition().x << ", " << slots[0].getPosition().y << "\n";
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
            slot.setItem(item, position);
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
    if (!visible) return;

    // Calculate the same offset as in draw()
    sf::Vector2f offset(
        position.x,
        position.y
    );

    if (!isDragging) {
        for (auto& slot : slots) {
            sf::FloatRect globalBounds = slot.getBounds();
            globalBounds.left += offset.x;
            globalBounds.top += offset.y;

            if (globalBounds.contains(mousePos)) {
                if (mouseDown && slot.isOccupied()) {
                    draggedItem = slot.getItem();
                    slot.clearItem();
                    isDragging = true;
                    break;
                }
            }
        }
    } else {
        if (mouseReleased) {
            for (auto& slot : slots) {
                sf::FloatRect globalBounds = slot.getBounds();
                globalBounds.left += offset.x;
                globalBounds.top += offset.y;

                if (globalBounds.contains(mousePos) && !slot.isOccupied()) {
                    slot.setItem(draggedItem, position);
                    draggedItem = nullptr;
                    isDragging = false;
                    return;
                }
            }

            for (auto& slot : slots) {
                if (!slot.isOccupied()) {
                    slot.setItem(draggedItem, position);
                    draggedItem = nullptr;
                    isDragging = false;
                    return;
                }
            }
        }
    }

    if (isDragging && draggedItem) {
        draggedItem->setDrawPosition(mousePos);
    }
}

