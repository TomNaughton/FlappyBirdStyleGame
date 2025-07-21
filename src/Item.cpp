#include "Item.hpp"
#include "iostream"

Item::Item(ItemType type, const std::string& name, const sf::Texture& texture)
    : type(type), name(name), texture(&texture)
{
    sprite.setTexture(texture);
    sprite.setScale(0.1f ,0.1f);
}

ItemType Item::getType() const {
    return type;
}

std::string Item::getName() const {
    return name;
}

void Item::setPosition(const sf::Vector2f& pos) {
    position = pos;
    std::cout << "New item position: " << position.x << ", " << position.y << std::endl;
}

void Item::draw(sf::RenderWindow& window) {
    if (texture) {
        sprite.setPosition(position);
        window.draw(sprite);
    }
    else {
    std::cout << "Item has no texture!!" << std::endl;
    }
}

sf::FloatRect Item::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2i Item::getSize() const {
    return size;
}

void Item::setTexture(const sf::Texture* tex) {
    texture = tex;
    sprite.setTexture(*texture);
}

void Item::setDrawPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(pos);
}