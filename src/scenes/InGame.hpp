#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <limits>

#include "../Player.hpp"
#include "../Asteroid.hpp"
#include "../DataManager.hpp"
#include "../BackgroundElementManager.hpp"
#include "../Projectile.hpp"

#include "../FastNoiseLite.h"

class InGame : public Scene {
public:
    InGame();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    std::string nextScene() const override;

    std::vector<Projectile> projectiles;
    void spawnProjectile(const sf::Vector2f& pos, const sf::Vector2f& velocity);
    void updateProjectiles(float dt);
    void drawProjectiles(sf::RenderWindow& window) const;

    static sf::RenderWindow* window;
    static sf::View* view;

    // void generateStarsAround(const sf::Vector2f& cameraCenter);
    // void cullFarChunks(const sf::Vector2f& cameraCenter);
    // sf::Color getStarColor(float parallaxFactor);
    void startShake(float intensity, float duration);

private:
    BackgroundElementManager backgroundElementManager;
    Player player;
    sf::Texture playerTexture;
    sf::Texture obstacleTexture;
    sf::Sprite asteroidSprite;
    sf::Sprite spaceshipSprite;
    sf::Texture spriteSheet;
    sf::Font font;
    sf::Vector2f previousViewCenter;
    FastNoiseLite noise;

    float currentZoom = 1.0f;
    float targetZoom = 1.0f;

    const float minZoom = 0.25f;
    const float maxZoom = 3.0f;
    
    

    sf::Vector2f shakeOffset = {0.f, 0.f};
    float shakeTime = 0.f;
    float shakeIntensity = 0.f;

    float dt;
};
