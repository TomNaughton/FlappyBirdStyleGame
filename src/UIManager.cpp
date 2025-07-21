// UIManager.cpp
#include "UIManager.hpp"

UIManager::UIManager(sf::RenderWindow& window) {
    uiView.setSize(window.getDefaultView().getSize());
    uiView.setCenter(window.getDefaultView().getCenter());
}

void UIManager::update(float dt) {}

void UIManager::draw(sf::RenderWindow& window) {
    // Set the view for UI before drawing any UI elements
    window.setView(uiView);

    // draw UI elements here
    inventory->draw(window);
}

void UIManager::onWindowResized(const sf::Vector2u& newSize) {
    uiView.setSize(sf::Vector2f(newSize));
    uiView.setCenter(sf::Vector2f(newSize.x / 2.f, newSize.y / 2.f));
}
