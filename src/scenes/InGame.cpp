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

    if (!font.loadFromFile("assets/arial.ttf")) {
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

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        sf::Vector2f playerPos = player.getPosition();

        float angleDegrees = player.getRotation();
        float angleRadians = angleDegrees * 3.14159265f / 180.f;
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

        float projectileSpeed = 600.f;
        float spawnOffset = 40.f; // spawn ahead of player
        sf::Vector2f spawnPos = playerPos + direction * spawnOffset;

        sf::Vector2f velocity = direction * projectileSpeed;
        projectiles.emplace_back(spawnPos, velocity);
    }
}

void InGame::update(float dt) {
    this->dt = dt;

    // simple linear interpolation
    float zoomSpeed = 5.0f; // higher = faster zoom
    currentZoom += (targetZoom - currentZoom) * dt * zoomSpeed;

    player.update(dt);
    updateProjectiles(dt);

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

    drawProjectiles(window);
    player.draw(window);
}

std::string InGame::nextScene() const {
    return "MainMenu";
}

void InGame::startShake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeTime = duration;
}

void InGame::updateProjectiles(float dt) {
    for (auto& p : projectiles) {
        p.update(dt);
    }
    // Remove dead projectiles
    projectiles.erase(
    std::remove_if(projectiles.begin(), projectiles.end(),
        [&](const Projectile& p) {
            return p.isOffScreen(player.getPosition(), maxZoom*1000);
        }),
    projectiles.end());
}

void InGame::drawProjectiles(sf::RenderWindow& window) const {
    for (const auto& p : projectiles) {
        p.draw(window);
    }
}
