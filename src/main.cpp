#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "asteroid.hpp"
#include "star.hpp"
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
    
    DataManager::Init();

    createStars(100, window.getSize());


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

    return 0;
}
