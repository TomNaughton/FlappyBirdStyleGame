#include "InGame.hpp"
#include "../datamanager.hpp"
#include <limits>

sf::RenderWindow* InGame::window = nullptr;

InGame::InGame() {
    if (!spriteSheet.loadFromFile("assets/spritesheet.png")) {
        // Handle loading error
    }

    spaceshipSprite.setTexture(spriteSheet);
    spaceshipSprite.setTextureRect(sf::IntRect(0, 0, 512, 270));
    spaceshipSprite.setPosition(50, window->getSize().y/2);
    player.setSprite(spaceshipSprite);

    asteroidSprite.setTexture(spriteSheet);
    asteroidSprite.setTextureRect(sf::IntRect(0, 400, 512, 512));

    if (!font.loadFromFile("assets/arial.ttf")) {
        // Handle loading error
    }

    createStars(100, window->getSize());
}

void InGame::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && player.hasJumpReset) {
        player.flap();
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
        player.hasJumpReset = true;
    }
}

void InGame::update(float dt) {
    this->dt = dt;
    if (gameOver) return;

    if (player.update(dt, window->getSize().y)) {
        if (score > DataManager::GetData().highScore) {
            DataManager::GetData().highScore = score;
            DataManager::Save();
        }
        gameOver = true;
    }
    
    obstacleSpawnTimer += dt;
    if (obstacleSpawnTimer >= spawnInterval) {
        obstacleSpawnTimer = 0.f;

        float x = window->getSize().x;
        float y = static_cast<float>(rand() % (window->getSize().y));
        float width = 100.f;
        float height = width;
        float speed = 200.f;

        obstacles.push_back(std::make_unique<Asteroid>(x, y, width, height, speed, asteroidSprite));
    }

    for (auto& obs : obstacles) {
        obs->update(dt);
    }

    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](auto& o) {
        return o->isOffScreen();
    }), obstacles.end());

    for (const auto& obs : obstacles) {
        float obstacleRight = obs->getPosition().x + obs->getWidth();
        float playerLeft = player.getBounds().left;

        if (!obs->passed && obstacleRight < playerLeft) {
            obs->passed = true;
            score++;
        }
    }

    for (auto& obs : obstacles) {
        if (obs->checkCollision(player.getBounds())) {
            gameOver = true;
            if (score > DataManager::GetData().highScore) {
                DataManager::GetData().highScore = score;
                DataManager::Save();
            }
            break;
        }
    }
}

void InGame::render(sf::RenderWindow& window) {
    for (auto& star : stars) {
        star.shape.move(-star.speed * dt, 0);
        if (star.shape.getPosition().x < 0)
            star.shape.setPosition(window.getSize().x, star.shape.getPosition().y);

        window.draw(star.shape);
    }

    player.draw(window);
    for (auto& obs : obstacles)
        obs->draw(window);

    sf::Text scoreText("Score: " + std::to_string(score), font, 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);

    sf::Text highScoreText("High Score: " + std::to_string(DataManager::GetData().highScore), font, 24);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setPosition(10, 40);
    window.draw(highScoreText);
}

bool InGame::isFinished() const {
    return gameOver;
}

std::string InGame::nextScene() const {
    return "MainMenu";
}

void InGame::createStars(int count, sf::Vector2u windowSize) {
    stars.clear();
    for (int i = 0; i < count; ++i) {
        Star star;
        star.shape = sf::CircleShape(1.f);
        star.shape.setFillColor(sf::Color::White);
        star.shape.setPosition(rand() % windowSize.x, rand() % windowSize.y);
        star.speed = 20.f + static_cast<float>(rand() % 40);
        stars.push_back(star);
    }
}