// UIManager.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "../Inventory.hpp"
#include "HUD.hpp"

namespace UI {
    class UIManager {
    public:
        UIManager(sf::RenderWindow& window);

        void update(float dt);
        void draw(sf::RenderWindow& window);

        void onWindowResized(const sf::Vector2u& newSize);
        void setInventory(Inventory* inv) { inventory = inv; }
        void setHUD(HUD* h) { hud = h; }

    private:
        sf::View uiView;
        Inventory* inventory;
        HUD* hud;
    };
}