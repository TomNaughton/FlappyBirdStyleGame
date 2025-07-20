#include "Asteroid.hpp"

Asteroid::Asteroid(const sf::Vector2f& position, float radius) {
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Asteroid::update(float dt) {
    shape.move(velocity * dt);

    // Implement destruction logic as needed
}

void Asteroid::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Asteroid::getPosition() const {
    return shape.getPosition();
}

void Asteroid::setVelocity(const sf::Vector2f& vel) {
    velocity = vel;
}

bool Asteroid::isDestroyed() const {
    return destroyed;
}

void Asteroid::markForRemoval() {
    toRemove = true;
}

bool Asteroid::isMarkedForRemoval() const {
    return toRemove;
}

sf::FloatRect Asteroid::getBounds() const {
    return shape.getGlobalBounds();
}