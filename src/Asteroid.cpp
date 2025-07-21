#include "Asteroid.hpp"
#include <iostream>

Asteroid::Asteroid(const sf::Vector2f& position, float radius, const sf::Texture& texture) : texture(&texture) {
    sprite.setOrigin(radius, radius);
    sprite.setPosition(position);
    sprite.setTexture(texture);
    sprite.setScale(radius / texture.getSize().x, radius / texture.getSize().y);
}

void Asteroid::update(float dt) {
    sprite.move(velocity * dt);
}

void Asteroid::draw(sf::RenderWindow& window) {
    if (texture) {
        window.draw(sprite);
    }
    else {
        std::cout << "Asteroid has no texture!" << std::endl;
    }
}

sf::Vector2f Asteroid::getPosition() const {
    return sprite.getPosition();
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
    return sprite.getGlobalBounds();
}