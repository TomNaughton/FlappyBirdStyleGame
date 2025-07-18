#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
public:
    Player();

    // Returns true if player hits top or bottom (game over)
    bool update(float dt, float windowHeight);
    void flap();
    void draw(sf::RenderWindow& window);
    void reset();
    void setSprite(sf::Sprite s);

    bool canJump = true;

    float getY() const;
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float velocity;
    float gravity;
    float flapStrength;
};
