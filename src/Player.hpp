#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

enum Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player {
public:
    Player();

    bool update(float dt);
    void flap();
    void draw(sf::RenderWindow& window);
    void reset();
    void setSprite(sf::Sprite s);
    void Move(float dt);

    sf::Vector2f getPosition() const;
    float getRotation() const;
    sf::FloatRect getBounds() const;

    void rotateLeft(float dt);
    void rotateRight(float dt);
    void thrustForward(float dt);
    void thrustBackward(float dt);

private:
    sf::Sprite sprite;
    sf::Texture texture;    
    
    float rotation = 0.f;          // degrees
    float speed = 0.f;             // current movement speed
    float maxSpeed = 300.f;        // tweak as needed
    float acceleration = 200.f;
    float deceleration = 150.f;

    sf::Vector2f velocity;
};
