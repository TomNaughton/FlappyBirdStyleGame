#include "WorldItem.hpp"

WorldItem::WorldItem(std::shared_ptr<Item> item_, const sf::Vector2f& position_)
    : item(item_), position(position_)
{
    item->setPosition(position);
}

void WorldItem::update(float dt) {
    // Future: simple floating, rotation, twinkle, etc.
    // For now, static position, no update
}

void WorldItem::draw(sf::RenderWindow& window) const {
    item->setPosition(position);
    item->draw(window);
}

const sf::Vector2f& WorldItem::getPosition() const {
    return position;
}

void WorldItem::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

std::shared_ptr<Item> WorldItem::getItem() const {
    return item;
}
