#include "NearbyItemsPanel.hpp"

NearbyItemsPanel::NearbyItemsPanel(int slotSize)
    : slotSize(slotSize) {
    panelBounds = sf::FloatRect(0.f, 0.f, slotSize * 5, slotSize); // single row of 5 slots
    slots.resize(5);

    for (int i = 0; i < 5; ++i) {
        slots[i].setSize({ static_cast<float>(slotSize), static_cast<float>(slotSize) });
        slots[i].setFillColor(sf::Color(40, 40, 40));
        slots[i].setOutlineThickness(2);
        slots[i].setOutlineColor(sf::Color(100, 100, 100));
        slots[i].setPosition(i * slotSize, 0.f);
    }
}

void NearbyItemsPanel::draw(sf::RenderWindow& window) {
    // Draw slots
    for (auto& slot : slots) {
        window.draw(slot);
    }

    // Draw items (not being dragged)
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] && items[i] != draggedItem) {
            items[i]->setPosition(slots[i].getPosition());
            items[i]->draw(window);
        }
    }

    // Draw dragged item on top
    if (draggedItem) {
        draggedItem->draw(window);
    }
}

void NearbyItemsPanel::update(float dt, const sf::Vector2f& mousePos, bool mousePressed, bool mouseReleased) {
    // Could add animations or hover effects here
}

void NearbyItemsPanel::setItems(const std::vector<std::shared_ptr<Item>>& newItems) {
    items.clear();
    for (int i = 0; i < 5 && i < (int)newItems.size(); ++i) {
        items.push_back(newItems[i]);
    }
    // Fill rest with nullptrs if less than 5
    while (items.size() < 5) {
        items.push_back(nullptr);
    }
}

std::shared_ptr<Item> NearbyItemsPanel::startDrag(const sf::Vector2f& mousePos) {
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i].getGlobalBounds().contains(mousePos) && items[i]) {
            draggedItem = items[i];
            items[i] = nullptr;
            draggedOffset = mousePos - slots[i].getPosition();
            return draggedItem;
        }
    }
    return nullptr;
}

bool NearbyItemsPanel::tryDrop(std::shared_ptr<Item> item, const sf::Vector2f& mousePos) {
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i].getGlobalBounds().contains(mousePos) && !items[i]) {
            items[i] = item;
            draggedItem = nullptr;
            return true;
        }
    }
    return false;
}
