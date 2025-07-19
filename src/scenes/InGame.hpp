#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "../Player.hpp"
#include "../Asteroid.hpp"
#include "../Star.cpp"

class InGame : public Scene {
public:
    InGame();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    std::string nextScene() const override;
    
    static sf::RenderWindow* window;
    static sf::View* view;

    void generateStarsAround(const sf::Vector2f& cameraCenter);
    std::vector<Star> generateStarsForChunk(int chunkX, int chunkY);
    void drawStars(sf::RenderWindow& window, const sf::Vector2f& cameraPos);
    void updateVisibleChunks(const sf::Vector2f& cameraCenter);

private:
    Player player;
    sf::Texture playerTexture;
    sf::Texture obstacleTexture;
    sf::Sprite asteroidSprite;
    sf::Sprite spaceshipSprite;
    sf::Texture spriteSheet;
    sf::Font font;
    sf::Vector2f previousViewCenter;
    
    std::vector<Star> stars;
    std::map<std::pair<int, int>, std::vector<Star>> starChunks;
    const float STAR_DENSITY = 0.0005f;
    const float STAR_AREA_RADIUS = 2000.f;
    const int CHUNK_SIZE = 512;

    float dt;
};
