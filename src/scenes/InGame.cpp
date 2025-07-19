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

    std::vector<sf::Vector2f> bigAsteroidShape = {
        { -40.f, -30.f },
        {  30.f, -25.f },
        {  45.f,  20.f },
        { -25.f,  40.f }
    };

    spaceObjects.spawnAsteroid(
        bigAsteroidShape,
        sf::Vector2f(500.f, 300.f),   // position
        sf::Vector2f(-50.f, 20.f),    // velocity
        30.f                          // rotation speed deg/s
    );

}

void InGame::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta < 0)
            targetZoom = std::min(targetZoom * 1.1f, maxZoom); // zoom out
        else
            targetZoom = std::max(targetZoom * 0.9f, minZoom); // zoom in
    }

    zoomOut = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    zoomIn = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
        
    //startShake(10.f, 0.4f); // shake with 10px intensity for 0.4 seconds

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        spaceObjects.spawnProjectile(player.getPosition(), player.getRotation(), 1000.0f);
    }
}

void InGame::update(float dt) {
    this->dt = dt;

    if (!zoomOut && zoomIn)
        targetZoom = std::max(targetZoom * 0.98f, minZoom);
    else if (zoomOut && !zoomIn)
        targetZoom = std::min(targetZoom * 1.02f, maxZoom);

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

    spaceObjects.update(dt, player.getPosition(), 2000.0f);
}

void InGame::render(sf::RenderWindow& window) {
    view->setCenter(view->getCenter() + shakeOffset);
    view->setSize(window.getDefaultView().getSize()); // base size
    view->zoom(currentZoom);
    window.setView(*view);

    backgroundElementManager.drawBackground(window, view->getCenter(), dt);
    backgroundElementManager.drawStars(window, view->getCenter(), dt);
    player.draw(window);
    spaceObjects.draw(window);
}

std::string InGame::nextScene() const {
    return "MainMenu";
}

void InGame::startShake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeTime = duration;
}