#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Obstacle {
public:
    virtual ~Obstacle() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool isOffScreen() const = 0;
    virtual bool checkCollision(const sf::FloatRect& other) const = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual float getWidth() const = 0;
};

class Asteroid : public Obstacle {
public:
    Asteroid(float x, float y, float width, float height, float speed, sf::Sprite s);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    bool isOffScreen() const override;
    bool checkCollision(const sf::FloatRect& other) const override;

    sf::Vector2f getPosition() const override;
    float getWidth() const override;

    bool passed = false;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
};
