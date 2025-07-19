#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Flappy Space");
    sf::View view(sf::FloatRect(0, 0, 1280, 720));
    
    window.setVerticalSyncEnabled(true);
    
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
            if (event.type == sf::Event::Resized) {
                // Get new window size
                float newWidth = static_cast<float>(event.size.width);
                float newHeight = static_cast<float>(event.size.height);

                // Calculate aspect ratio
                float windowAspect = newWidth / newHeight;
                float viewAspect = view.getSize().x / view.getSize().y;

                if (windowAspect > viewAspect) {
                    // Window is wider than view: add black bars on left/right
                    float viewportWidth = viewAspect / windowAspect;
                    view.setViewport(sf::FloatRect((1.f - viewportWidth) / 2.f, 0.f, viewportWidth, 1.f));
                } else {
                    // Window is taller than view: add black bars on top/bottom
                    float viewportHeight = windowAspect / viewAspect;
                    view.setViewport(sf::FloatRect(0.f, (1.f - viewportHeight) / 2.f, 1.f, viewportHeight));
                }
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
