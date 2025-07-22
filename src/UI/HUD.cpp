#include "HUD.hpp"

namespace UI {
    HUD::HUD(sf::RenderWindow& window) : window(window) {
        if (!font.loadFromFile("assets/arial.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        healthBarSize = {200, 20};
        healthBar.setSize(healthBarSize);
        healthBar.setFillColor(sf::Color::Green);
        healthBar.setDock(UI::UIElements::DOCK_BOTTOMLEFT);
        healthBar.setDockedPosition(0, fuelBar.getSize().y - healthBar.getSize().y); // Position above fuel bar

        fuelBarSize = {200, 20};
        fuelBar.setSize(fuelBarSize);
        fuelBar.setFillColor(sf::Color::Blue);
        fuelBar.setDock(UI::UIElements::DOCK_BOTTOMLEFT);
        fuelBar.setDockedPosition(0, 0);

        healthText.setFont(font);
        healthText.setCharacterSize(12);
        healthText.setFillColor(sf::Color::Black);
        healthText.setDock(UI::UIElements::DOCK_BOTTOMLEFT);
        healthText.setDockedPosition(0, fuelBar.getSize().y - healthBar.getSize().y); // Position above fuel bar

        fuelText.setFont(font);
        fuelText.setCharacterSize(12);
        fuelText.setFillColor(sf::Color::White);
        fuelText.setDock(UI::UIElements::DOCK_BOTTOMLEFT);
        fuelText.setDockedPosition(0, 0);

        setHealth(maxHealth);
        setFuel(maxFuel);
    }

    void HUD::update(float dt) {
        // Update health and fuel text
        healthText.setString("Health: " + std::to_string(health) + "/" + std::to_string(maxHealth));
        fuelText.setString("Fuel: " + std::to_string(fuel) + "/" + std::to_string(maxFuel));

        // Update health bar size
        float healthRatio = static_cast<float>(health) / maxHealth;
        healthBar.setSize({healthBarSize.x * healthRatio, healthBarSize.y});
        healthText.setDockedPosition(5, -fuelBar.getSize().y - (healthBar.getSize().y / 2));

        // Update fuel bar size
        float fuelRatio = static_cast<float>(fuel) / maxFuel;
        fuelBar.setSize({fuelBarSize.x * fuelRatio, fuelBarSize.y});
        fuelText.setDockedPosition(5, -fuelBar.getSize().y/2);
    }

    void HUD::draw(sf::RenderWindow& window) {
        healthBar.Draw(window);
        fuelBar.Draw(window);
        healthText.Draw(window);
        fuelText.Draw(window);
    }

    void HUD::setHealth(int health) {
        this->health = std::max(0, std::min(health, maxHealth));
    }

    void HUD::setMaxHealth(int maxHealth) {
        this->maxHealth = std::max(1, maxHealth);
        setHealth(health); // Ensure health is within new max
    }

    void HUD::setFuel(int fuel) {
        this->fuel = std::max(0, std::min(fuel, maxFuel));
    }

    void HUD::setMaxFuel(int maxFuel) {
        this->maxFuel = std::max(1, maxFuel);
        setFuel(fuel); // Ensure fuel is within new max
    }
}