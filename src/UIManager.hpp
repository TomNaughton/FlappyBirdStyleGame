// UIManager.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.hpp"

class UIManager {
public:
    UIManager(sf::RenderWindow& window);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void onWindowResized(const sf::Vector2u& newSize);
    void setInventory(Inventory* inv) { inventory = inv; }

private:
    sf::View uiView;
    Inventory* inventory;
};
