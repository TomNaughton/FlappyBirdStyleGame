#include "Player.hpp"
#include <cmath>

Player::Player() : inventory(5, 4, 64) {

}

void Player::update(float dt, SpaceObjectManager& spaceObjects) {
    Move(dt);
    attemptPickupNearbyItems(spaceObjects);
}

void Player::Move(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        rotateLeft(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        rotateRight(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        thrustForward(dt);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        thrustBackward(dt);
    else {
        if (speed > 0) {
            speed -= deceleration * dt;
            if (speed < 0) speed = 0;
        }
        else if (speed < 0) {
            speed += deceleration * dt;
            if (speed > 0) speed = 0;
        }
    }

    float rad = rotation * 3.1415926f / 180.f;
    velocity.x = std::cos(rad) * speed;
    velocity.y = std::sin(rad) * speed;

    sprite.move(velocity * dt);
    sprite.setRotation(rotation);
}

void Player::rotateLeft(float dt) {
    rotation -= 180.f * dt;
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
    inventory.draw(window);
}

void Player::setSprite(sf::Sprite s) {
    sprite = s;
    sprite.setScale(0.1f, 0.1f);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

float Player::getRotation() const {
    return rotation;
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

Inventory& Player::getInventory() {
    return inventory;
}

void Player::attemptPickupNearbyItems(SpaceObjectManager& spaceObjects) {
    float pickupRadius = 80.f;
    auto nearbyItems = spaceObjects.getItemsNearPosition(getPosition(), pickupRadius);

    for (auto& item : nearbyItems) {
        inventory.addItem(item);
    }
}

void Player::shoot(std::vector<Projectile>* projectiles) {
    float angleDegrees = sprite.getRotation();
    float angleRadians = angleDegrees * (3.14159265f / 180.0f);

    // Direction vector
    sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

    // Speed of projectile
    const float speed = 500.0f;

    Projectile projectile(sprite.getPosition(), direction * speed);
    // projectile.setRotation(angleDegrees);

    projectiles->push_back(projectile);
}