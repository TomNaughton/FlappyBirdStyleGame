// Projectile.cpp
#include "Projectile.hpp"

Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel)
    : velocity(vel) {
    shape.setRadius(4.f);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(pos);
}

void Projectile::update(float dt) {
    shape.move(velocity * dt);
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Projectile::isOffScreen(const sf::Vector2f& playerPos, float maxDistance) const {
    sf::Vector2f pos = shape.getPosition();
    float dx = pos.x - playerPos.x;
    float dy = pos.y - playerPos.y;
    float distanceSquared = dx * dx + dy * dy;
    return distanceSquared > maxDistance * maxDistance;
}

sf::Vector2f Projectile::getPosition() const {
    return shape.getPosition();
}