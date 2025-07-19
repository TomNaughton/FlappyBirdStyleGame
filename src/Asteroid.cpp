#include "Asteroid.hpp"

Asteroid::Asteroid(float x, float y, float width, float height, float speed, sf::Sprite s)
    : speed(speed) {
    
    sprite = s;
        
    sprite.setScale(width / sprite.getTextureRect().width, height / sprite.getTextureRect().height);

    sprite.setPosition(x, y);
}

void Asteroid::update(float dt) {
    sprite.move(-speed * dt, 0.f);
}

void Asteroid::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Asteroid::checkCollision(const sf::FloatRect& other) const {
    return sprite.getGlobalBounds().intersects(other);
}

sf::Vector2f Asteroid::getPosition() const {
    return sprite.getPosition();
}