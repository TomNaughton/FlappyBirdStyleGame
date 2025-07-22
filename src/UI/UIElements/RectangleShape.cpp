#include "RectangleShape.hpp"

namespace UI {
    namespace UIElements {
        
        void RectangleShape::Draw(sf::RenderWindow& window) {
            // Set the position based on the dock
            switch (getDock()) {
                case DOCK_TOPLEFT:
                    setPosition(getDockedPosition().x, getDockedPosition().y);
                    break;
                case DOCK_TOPRIGHT:
                    setPosition(window.getSize().x - getSize().x + getDockedPosition().x, getDockedPosition().y);
                    break;
                case DOCK_BOTTOMLEFT:
                    setPosition(getDockedPosition().x, window.getSize().y - getSize().y + getDockedPosition().y);
                    break;
                case DOCK_BOTTOMRIGHT:
                    setPosition(window.getSize().x - getSize().x + getDockedPosition().x, window.getSize().y - getSize().y + getDockedPosition().y);
                    break;
                case DOCK_CENTER:
                    setPosition(((window.getSize().x - getSize().x) / 2) + getDockedPosition().x, ((window.getSize().y - getSize().y) / 2) + getDockedPosition().y);
                    break;
            }
            window.draw(*this);
        }

    } // namespace UIElements
} // namespace UI