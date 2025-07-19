#include <SFML/Graphics.hpp>

struct Star {
    sf::Vector2f worldPos;
    float parallaxFactor; // between 0.1 and 0.5
    sf::Color color;
};
