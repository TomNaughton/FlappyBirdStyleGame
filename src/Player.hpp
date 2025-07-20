#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.hpp"
#include "SpaceObjectManager.hpp"
#include "Projectile.hpp"

class Player {
public:
    Player();

    void update(float dt, SpaceObjectManager& spaceObjects);
    void draw(sf::RenderWindow& window);

    void setSprite(sf::Sprite s);

    sf::Vector2f getPosition() const;
    float getRotation() const;
    sf::FloatRect getBounds() const;

    void shoot(std::vector<Projectile>* projectiles);

    Inventory& getInventory();

private:
    void Move(float dt);
    void rotateLeft(float dt);
    void rotateRight(float dt);
    void thrustForward(float dt);
    void thrustBackward(float dt);
    void attemptPickupNearbyItems(SpaceObjectManager& spaceObjects);

    sf::Sprite sprite;
    float rotation = 0.f;
    float speed = 0.f;
    float maxSpeed = 300.f;
    float acceleration = 200.f;
    float deceleration = 150.f;
    sf::Vector2f velocity;

    Inventory inventory;
};
