#pragma once
#include <SFML/Graphics.hpp>

class CameraShake {
public:
    void shake(float intensity, float duration);
    void update(float dt);
    sf::View applyTo(const sf::View& originalView);

private:
    float timeLeft = 0.f;
    float strength = 0.f;
};
