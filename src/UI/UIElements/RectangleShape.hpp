#pragma once
#include <SFML/Graphics.hpp>
#include "Dockable.hpp"

namespace UI {
    namespace UIElements {
        class RectangleShape : public sf::RectangleShape, public Dockable {
            public:
                void Draw(sf::RenderWindow& window) override;
        };
    }
}