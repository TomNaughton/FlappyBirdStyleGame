#include "InGame.hpp"
#include <cmath>
#include <random>
#include <cstdlib>

sf::RenderWindow* InGame::window = nullptr;
sf::View* InGame::view = nullptr;

InGame::InGame() : spaceObjects(worldItems), uiManager(*window) {
    uiManager.setInventory(&player.getInventory());

    if (!spaceshipTexture.loadFromFile("assets/spaceship.png")) {
        // Handle error
    }

    if (!asteroidTexture.loadFromFile("assets/asteroid.png")) {
        // Handle error
    }

    spaceshipSprite.setTexture(spaceshipTexture);
    spaceshipSprite.setPosition(50, window->getSize().y / 2);
    sf::FloatRect bounds = spaceshipSprite.getLocalBounds();
    spaceshipSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    player.setSprite(spaceshipSprite);

    if (!font.loadFromFile("assets/arial.ttf")) {
        // Handle error
    }

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.006f);
    noise.SetFractalOctaves(3);
    noise.SetFractalGain(0.5f);
    noise.SetFractalLacunarity(2.0f);
}

void InGame::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta < 0)
            targetZoom = std::min(targetZoom * 1.1f, maxZoom);
        else
            targetZoom = std::max(targetZoom * 0.9f, minZoom);
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            mouseReleased = true;
        }
    }

    zoomOut = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    zoomIn = sf::Keyboard::isKeyPressed(sf::Keyboard::X);

    if(event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::I) {
            player.getInventory().toggleVisibility();
        }
    }

    if (event.type == sf::Event::Resized) {
        sf::Vector2u newSize(event.size.width, event.size.height);
        view->setSize(newSize.x, newSize.y);
        view->setCenter(newSize.x / 2.f, newSize.y / 2.f);

        uiManager.onWindowResized(newSize);
    }
}

void InGame::update(float dt) {
    this->dt = dt;

    if (!zoomOut && zoomIn)
        targetZoom = std::max(targetZoom * 0.98f, minZoom);
    else if (zoomOut && !zoomIn)
        targetZoom = std::min(targetZoom * 1.02f, maxZoom);

    float zoomSpeed = 5.0f;
    currentZoom += (targetZoom - currentZoom) * dt * zoomSpeed;

    player.update(dt, spaceObjects);

    if (!player.getInventory().isVisible()) {
        float pickupRadius = 150.f;
        auto nearby = worldItems.getItemsNearPosition(player.getPosition(), pickupRadius);
        player.getInventory().setNearbyItems(nearby);
    }

    sf::Vector2f target = player.getPosition();
    sf::Vector2f current = view->getCenter();
    sf::Vector2f smoothed = current + (target - current) * 0.1f;

    view->setCenter(smoothed);
    window->setView(*view);

    backgroundElementManager.SpawnNewShootingStars(dt, view->getCenter());
    backgroundElementManager.EraseOldShootingStars(dt);
    backgroundElementManager.cullDistantChunks(view->getCenter());
    backgroundElementManager.updateVisibleChunks(view->getCenter(), view->getSize().x, view->getSize().y);

    visualEffects.update(dt);
    cameraShake.update(dt);

    spaceObjects.update(dt);
    worldItems.update(dt, player);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        static sf::Clock shootCooldown;
        if (shootCooldown.getElapsedTime().asMilliseconds() > 300) {
            player.shoot(&spaceObjects.getProjectiles());
            shootCooldown.restart();
        }
    }


    static float spawnTimer = 0.f;
    static constexpr float spawnInterval = 3.f;
    spawnTimer -= dt;
    if (spawnTimer <= 0.f) {
        spawnTimer = spawnInterval;

        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2u windowSize = window->getSize();

        float diagonal = std::sqrt(windowSize.x * windowSize.x + windowSize.y * windowSize.y);
        float maxViewDiagonal = diagonal * maxZoom;

        float spawnRadiusMin = maxViewDiagonal * 0.6f;
        float spawnRadiusMax = spawnRadiusMin + 100.f;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> angleDist(0.f, 2.f * 3.14159265f);
        std::uniform_real_distribution<float> radiusDist(spawnRadiusMin, spawnRadiusMax);

        float angle = angleDist(gen);
        float dist = radiusDist(gen);

        sf::Vector2f spawnPos = playerPos + sf::Vector2f(std::cos(angle), std::sin(angle)) * dist;

        float radius = 100.f + static_cast<float>(std::rand() % 20);

        Asteroid asteroid(spawnPos, radius, asteroidTexture);

        sf::Vector2f dir = playerPos - spawnPos;
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (length > 0.f)
            dir /= length;

        float speed = 10.f + static_cast<float>(std::rand() % 10);
        asteroid.setVelocity(dir * speed);

        spaceObjects.addAsteroid(asteroid);
    }

    if (mouseReleased) {
        player.getInventory().update(dt, static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)),
                                    sf::Mouse::isButtonPressed(sf::Mouse::Left),
                                    true);

        mouseReleased = false;
    }
    else {
        player.getInventory().update(dt, static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)),
                                    sf::Mouse::isButtonPressed(sf::Mouse::Left),
                                    false);
    }
}

void InGame::render(sf::RenderWindow& window) {
    view->setCenter(view->getCenter());
    view->setSize(window.getDefaultView().getSize());
    view->zoom(currentZoom);
    window.setView(*view);

    backgroundElementManager.drawBackground(window, view->getCenter(), dt);
    backgroundElementManager.drawStars(window, view->getCenter(), dt);
    player.draw(window);
    worldItems.draw(window);
    spaceObjects.draw(window);

    window.setView(cameraShake.applyTo(*view));

    visualEffects.draw(window);
    uiManager.draw(window);
}

std::string InGame::nextScene() const {
    return "MainMenu";
}

void InGame::startShake(float intensity, float duration) {
    cameraShake.shake(intensity, duration);
}
