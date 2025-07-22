#include "Text.hpp"

namespace UI {
    namespace UIElements {
        
        void Text::Draw(sf::RenderWindow& window) {
            // Set the position based on the dock
            switch(getDock()) {
                case DOCK_TOPLEFT:
                    setPosition(getDockedPosition().x, getDockedPosition().y);
                    break;
                case DOCK_TOPRIGHT:
                    setPosition(window.getSize().x - getGlobalBounds().width + getDockedPosition().x, getDockedPosition().y);
                    break;
                case DOCK_BOTTOMLEFT:
                    setPosition(getDockedPosition().x, window.getSize().y - getGlobalBounds().height + getDockedPosition().y);
                    break;
                case DOCK_BOTTOMRIGHT:
                    setPosition(window.getSize().x - getGlobalBounds().width + getDockedPosition().x, window.getSize().y - getGlobalBounds().height + getDockedPosition().y);
                    break;
                case DOCK_CENTER:
                    setPosition(((window.getSize().x - getGlobalBounds().width) / 2) + getDockedPosition().x, ((window.getSize().y - getGlobalBounds().height) / 2) + getDockedPosition().y);
                    break;
            }
            window.draw(*this);
        }

    } // namespace UIElements
} // namespace UI