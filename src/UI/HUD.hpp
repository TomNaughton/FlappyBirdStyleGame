#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "UIElements/RectangleShape.hpp"
#include "UIElements/Text.hpp"

namespace UI {
    class HUD {
        public:
            HUD(sf::RenderWindow& window);

            void update(float dt);
            void draw(sf::RenderWindow& window);

            void setHealth(int health);
            void setMaxHealth(int maxHealth);
            void setFuel(int fuel);
            void setMaxFuel(int maxFuel);

        private:
            sf::RenderWindow& window;

            UI::UIElements::Text healthText;
            UI::UIElements::Text fuelText;
            UI::UIElements::RectangleShape healthBar;
            UI::UIElements::RectangleShape fuelBar;
            
            sf::Font font;
            int health = 100;
            int maxHealth = 100;
            int fuel = 100;
            int maxFuel = 100;
            sf::Vector2f healthBarSize;
            sf::Vector2f fuelBarSize;
    };
}