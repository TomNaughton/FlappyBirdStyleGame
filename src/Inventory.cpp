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
    if(!visible) return;

    // Draw slots
    for (const auto& slot : slots) {
        slot.draw(window);
    }

    if (isDragging && draggedItem) {
        draggedItem->draw(window);
    }

    // Draw items inside slots
    drawItems(window);
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
    if (!visible) return;

    if (!isDragging) {
        // Not dragging anything, check if user clicked on a slot with an item
        for (auto& slot : slots) {
            if (slot.getBounds().contains(mousePos)) {
                if (mouseDown && slot.isOccupied()) {
                    draggedItem = slot.getItem();
                    draggedOffset = mousePos - slot.getPosition();
                    slot.clearItem();
                    isDragging = true;
                    break;
                }
            }
        }
    } else {
        // We're dragging an item
        if (mouseReleased) {
            // Try to drop into a slot
            for (auto& slot : slots) {
                if (slot.getBounds().contains(mousePos) && !slot.isOccupied()) {
                    slot.setItem(draggedItem);
                    draggedItem = nullptr;
                    isDragging = false;
                    return;
                }
            }

            // If no valid drop, cancel drag and drop back into first free slot
            for (auto& slot : slots) {
                if (!slot.isOccupied()) {
                    slot.setItem(draggedItem);
                    draggedItem = nullptr;
                    isDragging = false;
                    return;
                }
            }

            // Or optionally drop it on the floor
        }
    }

    // Optionally update dragged item position if it supports visual movement
    if (isDragging && draggedItem) {
        draggedItem->setDrawPosition(mousePos - draggedOffset);
    }
}
