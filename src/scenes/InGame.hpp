#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include "../VisualEffects.hpp"
#include "../CameraShake.hpp"
#include "../Player.hpp"
#include "../SpaceObjectManager.hpp"
#include "../BackgroundElementManager.hpp"
#include "../FastNoiseLite.h"
#include "../WorldItemManager.hpp"

class InGame : public Scene {
public:
    InGame();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    std::string nextScene() const override;

    static sf::RenderWindow* window;
    static sf::View* view;

    void startShake(float intensity, float duration);

private:
    BackgroundElementManager backgroundElementManager;
    Player player;
    sf::Sprite spaceshipSprite;
    sf::Texture spriteSheet;
    sf::Font font;
    FastNoiseLite noise;

    VisualEffects visualEffects;
    CameraShake cameraShake;

    float currentZoom = 1.0f;
    float targetZoom = 1.0f;
    bool zoomIn, zoomOut;

    const float minZoom = 0.25f;
    const float maxZoom = 3.0f;

    SpaceObjectManager spaceObjects;
    WorldItemManager worldItems;

    bool mouseReleased = false;

    float dt;
};
