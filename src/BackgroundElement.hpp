#pragma once
#include <SFML/Graphics.hpp>

#include <variant>

enum class BackgroundType {
    Star,
    Nebula,
    Planet,
    Comet,
    // Add more types later
};

struct BackgroundElement {
    sf::Vector2f worldPos;
    float parallaxFactor;

    BackgroundType type;

    sf::Color baseColor = sf::Color::White;

    std::variant<sf::CircleShape, sf::Sprite, sf::RectangleShape> drawable;
};

struct Star {
    sf::Vector2f worldPos;
    float parallaxFactor;
    float twinkleOffset;
    float size;
    sf::Color baseColor;
};

struct ShootingStar {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    float life;
    float parallaxFactor;
};
