#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "asteroid.hpp"
#include "star.hpp"
#include "datamanager.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib> // rand
#include <ctime>   // time
#include <fstream>
#include <string>
#include <limits>

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

std::vector<Star> stars;

void createStars(int count, sf::Vector2u windowSize) {
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

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Space");
    window.setFramerateLimit(60);

    DataManager dataManager;
    int* highScore = &dataManager.data.highScore;

    createStars(100, window.getSize());

    Player player;
    std::vector<std::unique_ptr<Asteroid>> obstacles;

    sf::Clock clock;
    float spawnTimer = 0.f;
    const float spawnInterval = 2.f;

    int score = 0;
    float lastScoreX = 0.f;

    sf::Texture spriteSheet;
    if (!spriteSheet.loadFromFile("assets/spritesheet.png")) {
        // Handle loading error
        return -1;
    }

    // Create spaceship sprite using left half of the texture
    sf::Sprite spaceshipSprite;
    spaceshipSprite.setTexture(spriteSheet);
    spaceshipSprite.setTextureRect(sf::IntRect(0, 0, 512, 270));
    player.setSprite(spaceshipSprite);

    // Create asteroid sprite using right half of the texture
    sf::Sprite asteroidSprite;
    asteroidSprite.setTexture(spriteSheet);
    asteroidSprite.setTextureRect(sf::IntRect(0, 400, 512, 512));

    GameState gameState = GameState::MENU;

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        
    }

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (gameState == GameState::MENU && event.key.code == sf::Keyboard::Space) {
                    gameState = GameState::PLAYING;
                    player.reset();
                    obstacles.clear();
                    score = 0;
                    lastScoreX = 0.f;
                    player.flap();
                }
                else if (gameState == GameState::PLAYING && event.key.code == sf::Keyboard::Space && player.canJump) {
                    player.flap();
                }
                else if (gameState == GameState::GAME_OVER && event.key.code == sf::Keyboard::Space) {
                    gameState = GameState::PLAYING;
                    player.reset();
                    obstacles.clear();
                    score = 0;
                    lastScoreX = 0.f;
                    player.flap();
                }
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                player.canJump = true;
            }
        }

        if (gameState == GameState::PLAYING) {
            if (player.update(dt, window.getSize().y)) {
                gameState = GameState::GAME_OVER;
                if (score > *highScore) {
                    *highScore = score;
                    dataManager.Save();
                }
            }

            spawnTimer += dt;
            if (spawnTimer >= spawnInterval) {
                spawnTimer = 0.f;

                float gapHeight = 200.f;
                float minHeight = 50.f;

                float x = window.getSize().x;
                float y = static_cast<float>(rand() % (window.getSize().y));
                float width = 0.1f;
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
                    gameState = GameState::GAME_OVER;
                    if (score > *highScore) {
                        *highScore = score;
                        dataManager.Save();
                    }
                    break;
                }
            }
        }

        window.clear();

        // Draw stars
        for (auto& star : stars) {
            star.shape.move(-star.speed * dt, 0);
            if (star.shape.getPosition().x < 0)
                star.shape.setPosition(window.getSize().x, star.shape.getPosition().y);

            window.draw(star.shape);
        }

        if (gameState == GameState::MENU) {
            sf::Text title("Flappy Space", font, 48);
            title.setFillColor(sf::Color::White);
            title.setPosition(200, 200);
            window.draw(title);

            sf::Text prompt("Press Space to Start", font, 24);
            prompt.setFillColor(sf::Color::Yellow);
            prompt.setPosition(270, 300);
            window.draw(prompt);
        }
        else if (gameState == GameState::PLAYING) {
            player.draw(window);
            for (auto& obs : obstacles)
                obs->draw(window);

            sf::Text scoreText("Score: " + std::to_string(score), font, 30);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);

            sf::Text highScoreText("High Score: " + std::to_string(*highScore), font, 24);
            highScoreText.setFillColor(sf::Color::White);
            highScoreText.setPosition(10, 40);
            window.draw(highScoreText);
        }
        else if (gameState == GameState::GAME_OVER) {
            player.draw(window);
            for (auto& obs : obstacles)
                obs->draw(window);

            sf::Text gameOverText("Game Over\nPress Space to Restart", font, 36);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setStyle(sf::Text::Bold);
            gameOverText.setPosition(200, 200);
            window.draw(gameOverText);

            sf::Text scoreText("Score: " + std::to_string(score), font, 30);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);

            sf::Text highScoreText("High Score: " + std::to_string(*highScore), font, 24);
            highScoreText.setFillColor(sf::Color::White);
            highScoreText.setPosition(10, 40);
            window.draw(highScoreText);
        }

        window.display();
    }

    return 0;
}
