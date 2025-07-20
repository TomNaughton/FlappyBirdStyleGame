#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Particle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
};

class VisualEffects {
public:
    void spawnImpactEffect(const sf::Vector2f& position, float asteroidArea);
    void spawnExplosionEffect(const sf::Vector2f& position, float asteroidArea);
    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<Particle> particles;
};
