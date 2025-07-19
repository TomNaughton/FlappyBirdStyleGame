#include "InGame.hpp"

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

    // generateStarsAround(spaceshipSprite.getPosition());

    player.setSprite(spaceshipSprite);


    asteroidSprite.setTexture(spriteSheet);
    asteroidSprite.setTextureRect(sf::IntRect(0, 400, 512, 512));

    if (!font.loadFromFile("../assets/arial.ttf")) {
        // Handle loading error
    }

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.006f);      // controls “scale” of clumps
    noise.SetFractalOctaves(3);      // adds detail
    noise.SetFractalGain(0.5f);
    noise.SetFractalLacunarity(2.0f);
}

void InGame::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta < 0)
            targetZoom = std::min(targetZoom * 1.1f, maxZoom); // zoom out
        else
            targetZoom = std::max(targetZoom * 0.9f, minZoom); // zoom in
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        targetZoom = std::max(targetZoom * 0.98f, minZoom);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        targetZoom = std::min(targetZoom * 1.02f, maxZoom);
        
    //startShake(10.f, 0.4f); // shake with 10px intensity for 0.4 seconds
}

void InGame::update(float dt) {
    this->dt = dt;

    // simple linear interpolation
    float zoomSpeed = 5.0f; // higher = faster zoom
    currentZoom += (targetZoom - currentZoom) * dt * zoomSpeed;

    player.update(dt);

    sf::Vector2f target = player.getPosition();
    sf::Vector2f current = view->getCenter();
    sf::Vector2f smoothed = current + (target - current) * 0.1f; // adjust 0.1f for smoothness

    view->setCenter(smoothed);
    window->setView(*view);

    //updateVisibleChunks(current);
    //cullFarChunks(current);

    backgroundElementManager.SpawnNewShootingStars(dt, view->getCenter());
    backgroundElementManager.EraseOldShootingStars(dt);
    backgroundElementManager.cullDistantChunks(view->getCenter());
    backgroundElementManager.updateVisibleChunks(view->getCenter(), view->getSize().x, view->getSize().y);


    if (shakeTime > 0.f) {
        shakeTime -= dt;

        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.1415926f;
        float magnitude = shakeIntensity * (shakeTime); // fades out

        shakeOffset = {
            std::cos(angle) * magnitude,
            std::sin(angle) * magnitude
        };
    } else {
        shakeOffset = {0.f, 0.f};
    }
}

void InGame::render(sf::RenderWindow& window) {
    view->setCenter(view->getCenter() + shakeOffset);
    view->setSize(window.getDefaultView().getSize()); // base size
    view->zoom(currentZoom);
    window.setView(*view);

    backgroundElementManager.drawBackground(window, view->getCenter(), dt);
    backgroundElementManager.drawStars(window, view->getCenter(), dt);
    //backgroundElementManager.drawDebugChunks(window, window.getView());

    player.draw(window);
}

std::string InGame::nextScene() const {
    return "MainMenu";
}

// void InGame::generateStarsAround(const sf::Vector2f& cameraCenter) {
//     stars.clear();

//     int numStars = static_cast<int>(STAR_AREA_RADIUS * STAR_AREA_RADIUS * STAR_DENSITY);
//     for (int i = 0; i < numStars; ++i) {
//         float x = cameraCenter.x + (rand() / (float)RAND_MAX - 0.5f) * 2 * STAR_AREA_RADIUS;
//         float y = cameraCenter.y + (rand() / (float)RAND_MAX - 0.5f) * 2 * STAR_AREA_RADIUS;
//         float parallax = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.4f;

//         Star star;
//         star.worldPos = {x, y};
//         star.parallaxFactor = parallax;
//         star.color = sf::Color::White;

//         stars.push_back(star);
//     }
// }

// std::vector<Star> InGame::generateStarsForChunk(int chunkX, int chunkY) {
//     std::vector<Star> chunkStars;

//     const float fx = chunkX * CHUNK_SIZE;
//     const float fy = chunkY * CHUNK_SIZE;
//     const int   CELL_SIZE = 32;    // size of sub‐cell to sample noise
//     const float threshold = 0.6f;  // tweak: higher = fewer stars

//     // iterate over a coarse grid and spawn with noise‐based probability
//     for (int gx = 0; gx < CHUNK_SIZE; gx += CELL_SIZE) {
//         for (int gy = 0; gy < CHUNK_SIZE; gy += CELL_SIZE) {
//             // world‐space sample point
//             float sampleX = fx + gx + CELL_SIZE * 0.5f;
//             float sampleY = fy + gy + CELL_SIZE * 0.5f;

//             // noise returns in [-1,1]; remap to [0,1]
//             float n = (noise.GetNoise(sampleX, sampleY) + 1.f) * 0.5f;
//             if (n < threshold) continue;

//             // within this cell, place a few stars proportional to noise
//             int starsInCell = static_cast<int>((n - threshold) / (1 - threshold) * 3);
//             for (int i = 0; i < starsInCell; ++i) {
//                 float wx = fx + gx + static_cast<float>(rand()) / RAND_MAX * CELL_SIZE;
//                 float wy = fy + gy + static_cast<float>(rand()) / RAND_MAX * CELL_SIZE;
//                 float parallax = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.4f;
//                 float twinkleOffset = static_cast<float>(rand()) / RAND_MAX * 6.28f;
//                 sf::Color color = getStarColor(parallax);

//                 chunkStars.push_back(Star{
//                     sf::Vector2f{wx, wy},
//                     parallax,
//                     color,
//                     twinkleOffset
//                 });
//             }
//         }
//     }

//     return chunkStars;
// }


// void InGame::updateVisibleChunks(const sf::Vector2f& cameraCenter) {
//     int minChunkX = static_cast<int>(std::floor((cameraCenter.x - STAR_AREA_RADIUS) / CHUNK_SIZE));
//     int maxChunkX = static_cast<int>(std::floor((cameraCenter.x + STAR_AREA_RADIUS) / CHUNK_SIZE));
//     int minChunkY = static_cast<int>(std::floor((cameraCenter.y - STAR_AREA_RADIUS) / CHUNK_SIZE));
//     int maxChunkY = static_cast<int>(std::floor((cameraCenter.y + STAR_AREA_RADIUS) / CHUNK_SIZE));

//     for (int cx = minChunkX; cx <= maxChunkX; ++cx) {
//         for (int cy = minChunkY; cy <= maxChunkY; ++cy) {
//             auto chunkKey = std::make_pair(cx, cy);
//             if (starChunks.find(chunkKey) == starChunks.end()) {
//                 starChunks[chunkKey] = generateStarsForChunk(cx, cy);
//             }
//         }
//     }
// }

// void InGame::drawStars(sf::RenderWindow& window, const sf::Vector2f& cameraPos) {
//     for (const auto& [chunkCoord, stars] : starChunks) {
//         for (const auto& star : stars) {
//             sf::Vector2f screenPos = star.worldPos - cameraPos * star.parallaxFactor;

//             float brightness = 0.6f + 0.4f * std::sin(dt * 2.f + star.twinkleOffset);
//             sf::Uint8 alpha = static_cast<sf::Uint8>(brightness * 255);

//             sf::CircleShape shape(1.0f);
//             shape.setPosition(screenPos);
//             shape.setFillColor(sf::Color(255, 255, 255, alpha));
//             window.draw(shape);
//         }
//     }
// }

// void InGame::cullFarChunks(const sf::Vector2f& cameraCenter) {
//     const float maxDistanceSq = STAR_AREA_RADIUS * STAR_AREA_RADIUS;

//     for (auto it = starChunks.begin(); it != starChunks.end(); ) {
//         const auto& [chunkX, chunkY] = it->first;

//         // Calculate the center of the chunk
//         float chunkCenterX = chunkX * CHUNK_SIZE + CHUNK_SIZE / 2.0f;
//         float chunkCenterY = chunkY * CHUNK_SIZE + CHUNK_SIZE / 2.0f;

//         float dx = chunkCenterX - cameraCenter.x;
//         float dy = chunkCenterY - cameraCenter.y;

//         float distanceSq = dx * dx + dy * dy;

//         if (distanceSq > maxDistanceSq) {
//             it = starChunks.erase(it); // erase returns the next iterator
//         } else {
//             ++it;
//         }
//     }
// }

// sf::Color InGame::getStarColor(float parallaxFactor) {
//     float t = (1.0f - parallaxFactor) / 0.9f; // 0 for far, 1 for near

//     // Simple blend from blue → white → orange
//     if (t < 0.5f) {
//         return sf::Color(
//             static_cast<sf::Uint8>(200 + 55 * t * 2),
//             static_cast<sf::Uint8>(200 + 55 * t * 2),
//             255
//         );
//     } else {
//         return sf::Color(
//             255,
//             static_cast<sf::Uint8>(255 - 100 * (t - 0.5f) * 2),
//             static_cast<sf::Uint8>(200 - 200 * (t - 0.5f) * 2)
//         );
//     }
// }

void InGame::startShake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeTime = duration;
}