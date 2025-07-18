#include "asteroid.hpp"

Asteroid::Asteroid(float x, float y, float width, float height, float speed, sf::Sprite s)
    : speed(speed) {
    
    sprite = s;
        
    // Scale the sprite to match the given width and height
    sprite.setScale(width, height);

    sprite.setPosition(x, y);
}

void Asteroid::update(float dt) {
    sprite.move(-speed * dt, 0.f);
}

void Asteroid::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Asteroid::isOffScreen() const {
    return sprite.getPosition().x + sprite.getGlobalBounds().width < 0;
}

bool Asteroid::checkCollision(const sf::FloatRect& other) const {
    return sprite.getGlobalBounds().intersects(other);
}

sf::Vector2f Asteroid::getPosition() const {
    return sprite.getPosition();
}

float Asteroid::getWidth() const {
    return sprite.getGlobalBounds().width;
}
