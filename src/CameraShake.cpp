#include "CameraShake.hpp"
#include <cstdlib>

void CameraShake::shake(float intensity, float duration) {
    strength = intensity;
    timeLeft = duration;
}

void CameraShake::update(float dt) {
    timeLeft -= dt;
    if (timeLeft < 0.f) timeLeft = 0.f;
}

sf::View CameraShake::applyTo(const sf::View& originalView) {
    if (timeLeft <= 0.f) return originalView;
    sf::View shaken = originalView;
    float offsetX = ((rand() % 100) / 100.f - 0.5f) * strength;
    float offsetY = ((rand() % 100) / 100.f - 0.5f) * strength;
    shaken.move(offsetX, offsetY);
    return shaken;
}
