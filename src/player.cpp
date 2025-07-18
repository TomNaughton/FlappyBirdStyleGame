#include "player.hpp"

Player::Player()
    : velocity(0.f), gravity(900.f), flapStrength(-350.f) {
        
}

bool Player::update(float dt, float windowHeight) {
    velocity += gravity * dt;
    sprite.move(0, velocity * dt);

    // Check if player hits floor or ceiling
    if (sprite.getPosition().y + sprite.getGlobalBounds().height > windowHeight) {
        sprite.setPosition(sprite.getPosition().x, windowHeight - sprite.getGlobalBounds().height);
        velocity = 0.f;
        return true; // hit bottom -> game over
    }
    if (sprite.getPosition().y < 0.f) {
        sprite.setPosition(sprite.getPosition().x, 0.f);
        velocity = 0.f;
        return true; // hit top -> game over
    }
    return false;
}

void Player::flap() {
    velocity = flapStrength;
    canJump = false;
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::reset() {
    sprite.setPosition(100.f, 300.f);
    velocity = 0.f;
}

float Player::getY() const {
    return sprite.getPosition().y;
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::setSprite(sf::Sprite s) {
    sprite = s;
    sprite.setScale(0.1f, 0.1f);
}