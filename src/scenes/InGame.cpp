#include "InGame.hpp"
#include "../DataManager.hpp"
#include <limits>

sf::RenderWindow* InGame::window = nullptr;
sf::View* InGame::view = nullptr;

InGame::InGame() {
    if (!spriteSheet.loadFromFile("assets/spritesheet.png")) {
        // Handle loading error
    }

    spaceshipSprite.setTexture(spriteSheet);
    spaceshipSprite.setTextureRect(sf::IntRect(0, 0, 512, 270));
    spaceshipSprite.setPosition(50, window->getSize().y/2);
    sf::FloatRect bounds = spaceshipSprite.getLocalBounds();
    spaceshipSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    generateStarsAround(spaceshipSprite.getPosition());

    player.setSprite(spaceshipSprite);


    asteroidSprite.setTexture(spriteSheet);
    asteroidSprite.setTextureRect(sf::IntRect(0, 400, 512, 512));

    if (!font.loadFromFile("assets/arial.ttf")) {
        // Handle loading error
    }
}

void InGame::handleEvent(const sf::Event& event) {
    
}

void InGame::update(float dt) {
    this->dt = dt;
    player.update(dt);

    sf::Vector2f target = player.getPosition();
    sf::Vector2f current = view->getCenter();
    sf::Vector2f smoothed = current + (target - current) * 0.1f; // adjust 0.1f for smoothness

    view->setCenter(smoothed);
    window->setView(*view);
}

void InGame::render(sf::RenderWindow& window) {
    updateVisibleChunks(view->getCenter());
    drawStars(window, view->getCenter());
    player.draw(window);
}

std::string InGame::nextScene() const {
    return "MainMenu";
}

void InGame::generateStarsAround(const sf::Vector2f& cameraCenter) {
    stars.clear();

    int numStars = static_cast<int>(STAR_AREA_RADIUS * STAR_AREA_RADIUS * STAR_DENSITY);
    for (int i = 0; i < numStars; ++i) {
        float x = cameraCenter.x + (rand() / (float)RAND_MAX - 0.5f) * 2 * STAR_AREA_RADIUS;
        float y = cameraCenter.y + (rand() / (float)RAND_MAX - 0.5f) * 2 * STAR_AREA_RADIUS;
        float parallax = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.4f;

        Star star;
        star.worldPos = {x, y};
        star.parallaxFactor = parallax;
        star.color = sf::Color::White;

        stars.push_back(star);
    }
}

std::vector<Star> InGame::generateStarsForChunk(int chunkX, int chunkY) {
    std::vector<Star> chunkStars;

    int area = CHUNK_SIZE * CHUNK_SIZE;
    int numStars = static_cast<int>(area * STAR_DENSITY);

    for (int i = 0; i < numStars; ++i) {
        float x = chunkX * CHUNK_SIZE + static_cast<float>(rand()) / RAND_MAX * CHUNK_SIZE;
        float y = chunkY * CHUNK_SIZE + static_cast<float>(rand()) / RAND_MAX * CHUNK_SIZE;
        float parallax = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.4f;

        chunkStars.push_back(Star{
            sf::Vector2f(x, y),
            parallax,
            sf::Color::White
        });
    }

    return chunkStars;
}

void InGame::updateVisibleChunks(const sf::Vector2f& cameraCenter) {
    int minChunkX = static_cast<int>(std::floor((cameraCenter.x - STAR_AREA_RADIUS) / CHUNK_SIZE));
    int maxChunkX = static_cast<int>(std::floor((cameraCenter.x + STAR_AREA_RADIUS) / CHUNK_SIZE));
    int minChunkY = static_cast<int>(std::floor((cameraCenter.y - STAR_AREA_RADIUS) / CHUNK_SIZE));
    int maxChunkY = static_cast<int>(std::floor((cameraCenter.y + STAR_AREA_RADIUS) / CHUNK_SIZE));

    for (int cx = minChunkX; cx <= maxChunkX; ++cx) {
        for (int cy = minChunkY; cy <= maxChunkY; ++cy) {
            auto chunkKey = std::make_pair(cx, cy);
            if (starChunks.find(chunkKey) == starChunks.end()) {
                starChunks[chunkKey] = generateStarsForChunk(cx, cy);
            }
        }
    }
}

void InGame::drawStars(sf::RenderWindow& window, const sf::Vector2f& cameraPos) {
    for (const auto& [chunkCoord, stars] : starChunks) {
        for (const auto& star : stars) {
            sf::Vector2f screenPos = star.worldPos - cameraPos * star.parallaxFactor;
            sf::CircleShape shape(1.0f);
            shape.setPosition(screenPos);
            shape.setFillColor(star.color);
            window.draw(shape);
        }
    }
}
