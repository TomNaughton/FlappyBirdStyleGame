#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.hpp"
#include "SpaceObjectManager.hpp"
#include "Projectile.hpp"
#include "UI/HUD.hpp"

class Player {
public:
    int inventoryWidth = 5;
    int inventoryHeight = 4;
    int slotSize = 64;
    Inventory inventory;
    UI::HUD hud;

    Player(sf::RenderWindow& window);

    void update(float dt, SpaceObjectManager& spaceObjects);
    void draw(sf::RenderWindow& window);

    void setSprite(sf::Sprite s);

    sf::Vector2f getPosition() const;
    float getRotation() const;
    sf::FloatRect getBounds() const;

    void shoot(std::vector<Projectile>* projectiles);

    Inventory* getInventory();
    UI::HUD* getHUD();

private:
    void Move(float dt);
    void rotateLeft(float dt);
    void rotateRight(float dt);
    void thrustForward(float dt);
    void thrustBackward(float dt);
    void attemptPickupNearbyItems(SpaceObjectManager& spaceObjects);

    sf::Sprite sprite;
    const sf::Texture* texture = nullptr; // pointer to your spritesheet texture
    sf::Vector2i frameSize = {32, 32}; // frame size in pixels (adjust to your frames)
    int frameCount = 3; // total frames in spritesheet
    int currentFrame = 0;
    sf::Time frameTime = sf::seconds(0.2f); // 200 ms per frame
    sf::Time elapsedTime = sf::Time::Zero;

    float rotation = 0.f;
    float speed = 0.f;
    float maxSpeed = 300.f;
    float acceleration = 200.f;
    float deceleration = 150.f;
    sf::Vector2f velocity;
};
