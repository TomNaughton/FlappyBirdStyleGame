#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class ItemType {
    Resource,
    // Extend with other types later
};

class Item {
public:
    Item(ItemType type, const std::string& name, const sf::Texture& texture);

    ItemType getType() const;
    std::string getName() const;

    void setPosition(const sf::Vector2f& pos);
    void setDrawPosition(const sf::Vector2f& pos);
    void setTexture(const sf::Texture* tex);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    sf::Sprite* getSprite();

    // Inventory size and shape
    sf::Vector2i getSize() const;

private:
    ItemType type;
    std::string name;
    sf::Sprite sprite;
    const sf::Texture* texture = nullptr;
    sf::Vector2f position;

    sf::Vector2i size = {1, 1}; // Default occupies 1 slot
};
