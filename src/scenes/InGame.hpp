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
#include "../UI/UIManager.hpp"

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

    sf::Texture spaceshipTexture;
    sf::Texture asteroidTexture;
    sf::Sprite spaceshipSprite;
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
    UI::UIManager uiManager;

    bool mouseReleased = false;

    sf::Vector2i spaceshipFrameSize;
    int spaceshipCurrentFrame;
    sf::Time spaceshipFrameTime;
    sf::Time spaceshipElapsedTime;

    float dt;
};
