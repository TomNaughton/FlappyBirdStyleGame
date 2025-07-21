#include "Player.hpp"
#include <cmath>

Player::Player(sf::RenderWindow& window) : inventory(inventoryWidth, inventoryHeight, slotSize) {
    sf::Vector2u windowSize = window.getSize();
    inventory.position = sf::Vector2f(
        (windowSize.x - inventoryWidth * slotSize) / 2.f,
        (windowSize.y - inventoryHeight * slotSize) / 2.f
    );
}

void Player::update(float dt, SpaceObjectManager& spaceObjects) {
    Move(dt);
    attemptPickupNearbyItems(spaceObjects);

    // Animate sprite frames
    elapsedTime += sf::seconds(dt);
    if (elapsedTime >= frameTime) {
        elapsedTime -= frameTime;
        currentFrame = (currentFrame + 1) % frameCount;

        int top = currentFrame * frameSize.y;
        sprite.setTextureRect(sf::IntRect(0, top, frameSize.x, frameSize.y));
    }
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
}

void Player::setSprite(sf::Sprite s) {
    sprite = s;
    texture = sprite.getTexture(); // store pointer to texture (spritesheet)
    sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y)); // initial frame rect
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f); // set origin to center of sprite
    sprite.setScale(2.f, 2.f); // scale sprite to desired size
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

Inventory* Player::getInventory() {
    return &inventory;
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