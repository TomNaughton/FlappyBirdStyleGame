#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "../player.hpp"
#include "../asteroid.hpp"

class InGame : public Scene {
public:
    InGame();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isFinished() const override;
    std::string nextScene() const override;
    
    static sf::RenderWindow* window;

private:
    Player player;
    std::vector<std::unique_ptr<Asteroid>> obstacles;
    sf::Texture playerTexture;
    sf::Texture obstacleTexture;
    sf::Sprite asteroidSprite;
    sf::Sprite spaceshipSprite;
    sf::Texture spriteSheet;
    sf::Font font;

    float obstacleSpawnTimer = 0.f;
    float spawnInterval = 2.0f;

    int score = 0;
    float lastScoreX = 0.f;

    sf::Text scoreText;

    bool gameOver = false;
};
