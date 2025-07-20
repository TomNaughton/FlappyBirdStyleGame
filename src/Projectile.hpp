#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(const sf::Vector2f& position, const sf::Vector2f& velocity, float damage = 10.f, float radius = 5.f);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    const sf::Vector2f& getPosition() const;
    float getRadius() const;
    float getDamage() const;

    bool isExpired() const;

    bool shouldBeRemoved() const;
    void markForRemoval();
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float damage;

    float lifetime = 0.f;
    float maxLifetime = 10.f; // seconds
    bool toRemove = false;
};
