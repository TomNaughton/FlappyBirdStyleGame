#include <SFML/Graphics.hpp>
#include "scenes/SceneManager.hpp"
#include "DataManager.hpp"
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
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    window.setFramerateLimit(60);
    
    DataManager::Init();

    sf::Clock clock;

    InGame::window = &window;
    InGame::view = &view;

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
