#include "VisualEffects.hpp"
#include <cmath>
#include <algorithm>

void VisualEffects::spawnImpactEffect(const sf::Vector2f& position, float area) {
    int count = std::clamp(static_cast<int>(area * 0.02f), 5, 20);
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.shape.setRadius(1.5f);
        p.shape.setFillColor(sf::Color(200, 200, 200));
        p.shape.setPosition(position);
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f;
        float speed = 30.f + static_cast<float>(rand()) / RAND_MAX * 70.f;
        p.velocity = { std::cos(angle) * speed, std::sin(angle) * speed };
        p.lifetime = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.3f;
        particles.push_back(p);
    }
}

void VisualEffects::spawnExplosionEffect(const sf::Vector2f& position, float area) {
    int count = std::clamp(static_cast<int>(area * 0.04f), 10, 40);
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.shape.setRadius(2.f + static_cast<float>(rand()) / RAND_MAX * 2.f);
        p.shape.setFillColor(sf::Color(255, 100, 50));
        p.shape.setPosition(position);
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f;
        float speed = 60.f + static_cast<float>(rand()) / RAND_MAX * 120.f;
        p.velocity = { std::cos(angle) * speed, std::sin(angle) * speed };
        p.lifetime = 0.8f + static_cast<float>(rand()) / RAND_MAX * 0.4f;
        particles.push_back(p);
    }
}

void VisualEffects::update(float dt) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->lifetime -= dt;
        if (it->lifetime <= 0) {
            it = particles.erase(it);
        } else {
            it->shape.move(it->velocity * dt);
            sf::Color color = it->shape.getFillColor();
            color.a = static_cast<sf::Uint8>(255 * (it->lifetime / 1.5f));
            it->shape.setFillColor(color);
            ++it;
        }
    }
}

void VisualEffects::draw(sf::RenderWindow& window) {
    for (auto& p : particles) {
        window.draw(p.shape);
    }
}
