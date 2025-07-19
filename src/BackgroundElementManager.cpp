#include "BackgroundElementManager.hpp"
#include <random>

BackgroundElementManager::BackgroundElementManager() {
    debugFont.loadFromFile("../assets/arial.ttf");
}

void BackgroundElementManager::generateBackgroundForChunk(const ChunkCoord& coord, std::vector<BackgroundElement>& outElements) {
    sf::Vector2f chunkOrigin(coord.x * CHUNK_SIZE, coord.y * CHUNK_SIZE);

    // Add stars
    for (int i = 0; i < 20; ++i) {
        sf::Vector2f pos = chunkOrigin + sf::Vector2f(
            static_cast<float>(rand()) / RAND_MAX * CHUNK_SIZE,
            static_cast<float>(rand()) / RAND_MAX * CHUNK_SIZE
        );
        outElements.push_back(createStar(pos));
    }
}


void BackgroundElementManager::updateVisibleChunks(const sf::Vector2f& cameraPos, float viewWidth, float viewHeight) {
    ChunkCoord center = getChunkCoord(cameraPos);

    for (int dx = -BUFFER_CHUNK_RADIUS; dx <= BUFFER_CHUNK_RADIUS; ++dx) {
        for (int dy = -BUFFER_CHUNK_RADIUS; dy <= BUFFER_CHUNK_RADIUS; ++dy) {
            ChunkCoord coord{center.x + dx, center.y + dy};

            if (backgroundChunks.find(coord) == backgroundChunks.end()) {
                std::vector<BackgroundElement> elements;
                generateBackgroundForChunk(coord, elements);
                backgroundChunks[coord] = std::move(elements);
            }
        }
    }
}

void BackgroundElementManager::cullDistantChunks(const sf::Vector2f& cameraPos) {
    ChunkCoord center = getChunkCoord(cameraPos);

    for (auto it = backgroundChunks.begin(); it != backgroundChunks.end(); ) {
        int dx = std::abs(it->first.x - center.x);
        int dy = std::abs(it->first.y - center.y);

        if (dx > BUFFER_CHUNK_RADIUS || dy > BUFFER_CHUNK_RADIUS) {
            it = backgroundChunks.erase(it);
        } else {
            ++it;
        }
    }
}

void BackgroundElementManager::drawBackgroundElement(sf::RenderWindow& window, BackgroundElement& elem, const sf::Vector2f& cameraPos) {
    sf::Vector2f screenPos = (elem.worldPos - cameraPos) * elem.parallaxFactor + cameraPos;

    std::visit([&](auto& shape) {  // remove const here
        shape.setPosition(screenPos);
        window.draw(shape);
    }, elem.drawable);
}


void BackgroundElementManager::drawBackground(sf::RenderWindow& window, const sf::Vector2f& cameraPos, float time) {
    starVertexArray.clear();

    for (auto& [chunkCoord, elements] : backgroundChunks) {
        for (auto& elem : elements) {
            if (elem.type == BackgroundType::Star) {
                sf::Vector2f screenPos = (elem.worldPos - cameraPos) * elem.parallaxFactor + cameraPos;

                sf::CircleShape* circle = std::get_if<sf::CircleShape>(&elem.drawable);
                if (!circle) continue;

                float brightness = 0.6f + 0.4f * std::sin(time * 2.f + elem.worldPos.x + elem.worldPos.y);
                sf::Uint8 alpha = static_cast<sf::Uint8>(brightness * 255);

                sf::Color baseColor = circle->getFillColor();
                sf::Color color(baseColor.r, baseColor.g, baseColor.b, alpha);

                float radius = circle->getRadius();
                float halfSize = radius;

                // Add 4 vertices to starVertexArray to form a quad (square) for the star
                starVertexArray.append(sf::Vertex(screenPos + sf::Vector2f(-halfSize, -halfSize), color));
                starVertexArray.append(sf::Vertex(screenPos + sf::Vector2f(halfSize, -halfSize), color));
                starVertexArray.append(sf::Vertex(screenPos + sf::Vector2f(halfSize, halfSize), color));
                starVertexArray.append(sf::Vertex(screenPos + sf::Vector2f(-halfSize, halfSize), color));

            } else {
                // Non-star elements: draw normally
                sf::Vector2f screenPos = (elem.worldPos - cameraPos) * elem.parallaxFactor + cameraPos;

                std::visit([&](auto& drawable) {
                    drawable.setPosition(screenPos);
                    window.draw(drawable);
                }, elem.drawable);
            }
        }
    }

    // Draw all stars at once
    window.draw(starVertexArray);
}


void BackgroundElementManager::drawStars(sf::RenderWindow& window, 
                                         const sf::Vector2f& cameraPos, 
                                         float time) {
    sf::VertexArray starVertices(sf::Points);

    for (const auto& [chunkCoord, elements] : backgroundChunks) {
        for (const auto& elem : elements) {
            if (elem.type != BackgroundType::Star) continue;

            sf::Vector2f screenPosF = (elem.worldPos - cameraPos) * elem.parallaxFactor + cameraPos;
            sf::Vector2i screenPosI(static_cast<int>(screenPosF.x + 0.5f), static_cast<int>(screenPosF.y + 0.5f));
            sf::Vector2f screenPos(static_cast<float>(screenPosI.x), static_cast<float>(screenPosI.y));

            // twinkle brightness 0.6..1.0 range
            float brightness = 0.6f + 0.4f * std::sin(time * 2.f + elem.worldPos.x + elem.worldPos.y);
            sf::Uint8 alpha = static_cast<sf::Uint8>(brightness * 255);

            sf::Color color = elem.baseColor;
            color.a = alpha;

            starVertices.append(sf::Vertex(screenPos, color));
        }
    }

    window.draw(starVertices);
}

void BackgroundElementManager::drawDebugChunks(sf::RenderWindow& window, const sf::View& view) {
    ChunkCoord center = getChunkCoord(view.getCenter());

    for (int dx = -BUFFER_CHUNK_RADIUS; dx <= BUFFER_CHUNK_RADIUS; ++dx) {
        for (int dy = -BUFFER_CHUNK_RADIUS; dy <= BUFFER_CHUNK_RADIUS; ++dy) {
            ChunkCoord coord{center.x + dx, center.y + dy};
            sf::Vector2f chunkOrigin(coord.x * CHUNK_SIZE, coord.y * CHUNK_SIZE);

            // Draw chunk rectangle
            sf::RectangleShape rect(sf::Vector2f(CHUNK_SIZE, CHUNK_SIZE));
            rect.setPosition(chunkOrigin);
            rect.setFillColor(sf::Color::Transparent);

            int dist = std::max(std::abs(dx), std::abs(dy));
            rect.setOutlineColor(dist <= VISIBLE_CHUNK_RADIUS ? sf::Color::Green : sf::Color(100, 100, 255));
            rect.setOutlineThickness(1.f);
            window.draw(rect);

            // Draw chunk coordinates
            sf::Text label;
            label.setFont(debugFont);
            label.setCharacterSize(100);
            label.setFillColor(sf::Color::White);
            label.setString(std::to_string(coord.x) + "," + std::to_string(coord.y));
            label.setPosition(chunkOrigin.x + 4.f, chunkOrigin.y + 4.f);
            window.draw(label);

            // Draw element dots
            auto it = backgroundChunks.find(coord);
            if (it != backgroundChunks.end()) {
                for (const auto& elem : it->second) {
                    sf::CircleShape dot(2.f);
                    sf::Vector2f screenPos = (elem.worldPos - view.getCenter()) * elem.parallaxFactor + view.getCenter();
                    dot.setPosition(screenPos);
                    dot.setFillColor(sf::Color::Yellow);
                    window.draw(dot);
                }
            }
        }
    }
}

void BackgroundElementManager::SpawnNewShootingStars(float dt, const sf::Vector2f& cameraPos) {
    static float shootingStarTimer = 0.f;
    shootingStarTimer -= dt;

    if (shootingStarTimer <= 0.f) {
        shootingStarTimer = 10.f + static_cast<float>(rand()) / RAND_MAX * 15.f;

        ShootingStar s;
        s.pos = cameraPos + sf::Vector2f(-400.f, -300.f); // top-left of screen
        s.velocity = sf::Vector2f(300.f, 300.f); // diagonal
        s.life = 1.0f;
        s.parallaxFactor = 0.2f;

        shootingStars.push_back(s);
    }
}

void BackgroundElementManager::EraseOldShootingStars(float dt) {
    for (auto it = shootingStars.begin(); it != shootingStars.end(); ) {
        it->pos += it->velocity * dt;
        it->life -= dt;

        if (it->life <= 0.f)
            it = shootingStars.erase(it);
        else
            ++it;
    }
}

void BackgroundElementManager::DrawShootingStars(sf::RenderWindow& window, const sf::Vector2f& cameraPos) {
    for (const auto& star : shootingStars) {
        sf::Vector2f screenPos = star.pos - cameraPos * star.parallaxFactor;

        sf::RectangleShape streak(sf::Vector2f(30.f, 2.f));
        streak.setPosition(screenPos);
        streak.setRotation(45.f); // direction
        streak.setFillColor(sf::Color(255, 255, 255, 180));
        window.draw(streak);
    }
}

ChunkCoord BackgroundElementManager::getChunkCoord(const sf::Vector2f& pos) {
    return {
        static_cast<int>(std::floor(pos.x / CHUNK_SIZE)),
        static_cast<int>(std::floor(pos.y / CHUNK_SIZE))
    };
}

BackgroundElement BackgroundElementManager::createStar(const sf::Vector2f& pos) {
    BackgroundElement star;
    star.worldPos = pos;
    star.parallaxFactor = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.4f;
    star.type = BackgroundType::Star;
    star.baseColor = sf::Color::White;

    return star;
}
