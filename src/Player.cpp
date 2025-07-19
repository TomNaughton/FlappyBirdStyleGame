#include "Player.hpp"

Player::Player() {
        
}

bool Player::update(float dt) {
    Move(dt);
    return false;
}

void Player::Move(float dt) {
    // Apply rotation input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        rotateLeft(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        rotateRight(dt);

    // Apply movement input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        thrustForward(dt);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        thrustBackward(dt);
    else {
        // Apply natural deceleration when no input
        if (speed > 0) {
            speed -= deceleration * dt;
            if (speed < 0) speed = 0;
        } else if (speed < 0) {
            speed += deceleration * dt;
            if (speed > 0) speed = 0;
        }
    }

    // Convert speed + rotation to a movement vector
    float rad = rotation * 3.1415926f / 180.f;
    velocity.x = std::cos(rad) * speed;
    velocity.y = std::sin(rad) * speed;

    // Move the sprite
    sprite.move(velocity * dt);
    sprite.setRotation(rotation);
}

void Player::rotateLeft(float dt) {
    rotation -= 180.f * dt; // 180 degrees/sec
    if (rotation < 0) rotation += 360.f;
}

void Player::rotateRight(float dt) {
    rotation += 180.f * dt;
    if (rotation >= 360.f) rotation -= 360.f;
}

void Player::thrustForward(float dt) {
    speed += acceleration * dt;
    if (speed > maxSpeed) speed = maxSpeed;
}

void Player::thrustBackward(float dt) {
    speed -= acceleration * dt;
    if (speed < -maxSpeed / 2.f) speed = -maxSpeed / 2.f;
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::reset() {
    sprite.setPosition(0.f, 0.f);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::setSprite(sf::Sprite s) {
    sprite = s;
    sprite.setScale(0.1f, 0.1f);
}