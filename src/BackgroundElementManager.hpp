#pragma once

#include <cmath>
#include "BackgroundElement.hpp"

struct ChunkCoord {
    int x, y;

    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y;
    }

    friend bool operator<(const ChunkCoord& a, const ChunkCoord& b) {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }
};

// Hash function
namespace std {
    template<>
    struct hash<ChunkCoord> {
        std::size_t operator()(const ChunkCoord& coord) const {
            return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
        }
    };
}

class BackgroundElementManager {
public:
    BackgroundElementManager();

    void generateBackgroundForChunk(const ChunkCoord& coord, std::vector<BackgroundElement>& outElements);
    void updateVisibleChunks(const sf::Vector2f& cameraPos, float viewWidth, float viewHeight);
    void cullDistantChunks(const sf::Vector2f& cameraPos);
    void drawBackgroundElement(sf::RenderWindow& window, BackgroundElement& elem, const sf::Vector2f& cameraPos);
    void drawBackground(sf::RenderWindow& window, const sf::Vector2f& cameraPos, float time);
    void drawStars(sf::RenderWindow& window, const sf::Vector2f& cameraPos, float time);
    void drawDebugChunks(sf::RenderWindow& window, const sf::View& view);
    ChunkCoord getChunkCoord(const sf::Vector2f& pos);

    // Shooting stars
    void SpawnNewShootingStars(float dt, const sf::Vector2f& cameraPos);
    void EraseOldShootingStars(float dt);
    void DrawShootingStars(sf::RenderWindow& window, const sf::Vector2f& cameraPos);

    BackgroundElement createStar(const sf::Vector2f& pos);

private:
    std::map<ChunkCoord, std::vector<BackgroundElement>> backgroundChunks;
    const float CHUNK_RADIUS = 2000.f;
    const int CHUNK_SIZE = 512;
    const int VISIBLE_CHUNK_RADIUS = 7;
    const int BUFFER_CHUNK_RADIUS = VISIBLE_CHUNK_RADIUS + 2;


    std::vector<ShootingStar> shootingStars;
    float shootingStarTimer = 0.f;

    std::vector<Star> stars;
    sf::VertexArray starVertexArray{sf::Quads};

    sf::Font debugFont;
};
