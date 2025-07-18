#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "asteroid.hpp"
#include "datamanager.hpp"
#include "scenes/SceneManager.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib> // rand
#include <ctime>   // time
#include <fstream>
#include <string>
#include <limits>

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Space");
    window.setFramerateLimit(60);
    
    DataManager::Init();

    sf::Clock clock;

    InGame::window = &window;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            SceneManager::getCurrentScene()->handleEvent(event);
        }

        SceneManager::getCurrentScene()->update(dt);

        if (SceneManager::getCurrentScene()->isFinished()) {
            std::string next = SceneManager::getCurrentScene()->nextScene();
            SceneManager::changeScene(next);
        }

        window.clear();
        SceneManager::getCurrentScene()->render(window);
        window.display();
    }

    //             else if (gameState == GameState::GAME_OVER && event.key.code == sf::Keyboard::Space) {
    //                 gameState = GameState::PLAYING;
    //                 player.reset();
    //                 obstacles.clear();
    //                 score = 0;
    //                 lastScoreX = 0.f;
    //                 player.flap();
    //             }
    //         }
    //     }


    //         
    //     }

    //     window.clear();

    //     // Draw stars
    //     for (auto& star : stars) {
    //         star.shape.move(-star.speed * dt, 0);
    //         if (star.shape.getPosition().x < 0)
    //             star.shape.setPosition(window.getSize().x, star.shape.getPosition().y);

    //         window.draw(star.shape);
    //     }

    //     if (gameState == GameState::MENU) {
    //         sf::Text title("Flappy Space", font, 48);
    //         title.setFillColor(sf::Color::White);
    //         title.setPosition(200, 200);
    //         window.draw(title);

    //         sf::Text prompt("Press Space to Start", font, 24);
    //         prompt.setFillColor(sf::Color::Yellow);
    //         prompt.setPosition(270, 300);
    //         window.draw(prompt);
    //     }
    //    
    //     else if (gameState == GameState::GAME_OVER) {
    //         player.draw(window);
    //         for (auto& obs : obstacles)
    //             obs->draw(window);

    //         sf::Text gameOverText("Game Over\nPress Space to Restart", font, 36);
    //         gameOverText.setFillColor(sf::Color::Red);
    //         gameOverText.setStyle(sf::Text::Bold);
    //         gameOverText.setPosition(200, 200);
    //         window.draw(gameOverText);

    //         sf::Text scoreText("Score: " + std::to_string(score), font, 30);
    //         scoreText.setFillColor(sf::Color::White);
    //         scoreText.setPosition(10, 10);
    //         window.draw(scoreText);

    //         sf::Text highScoreText("High Score: " + std::to_string(*highScore), font, 24);
    //         highScoreText.setFillColor(sf::Color::White);
    //         highScoreText.setPosition(10, 40);
    //         window.draw(highScoreText);
    //     }

    //     window.display();
    // }

    return 0;
}
