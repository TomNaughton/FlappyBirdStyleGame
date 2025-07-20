#include "Projectile.hpp"

Projectile::Projectile(const sf::Vector2f& position, const sf::Vector2f& velocity, float damage, float radius)
    : velocity(velocity), damage(damage), lifetime(0.f), maxLifetime(10.f)
{
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Yellow);
}

void Projectile::update(float dt) {
    shape.move(velocity * dt);
    lifetime += dt;
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

const sf::Vector2f& Projectile::getPosition() const {
    return shape.getPosition();
}

float Projectile::getRadius() const {
    return shape.getRadius();
}

float Projectile::getDamage() const {
    return damage;
}

bool Projectile::isExpired() const {
    return lifetime >= maxLifetime;
}

bool Projectile::shouldBeRemoved() const {
    return toRemove;
}

void Projectile::markForRemoval() {
    toRemove = true;
}

sf::FloatRect Projectile::getBounds() const {
    return shape.getGlobalBounds();  // Assuming you're using a CircleShape or similar
}
