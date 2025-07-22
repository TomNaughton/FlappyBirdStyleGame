#pragma once
#include <SFML/Graphics.hpp>
#include "Dockable.hpp"

namespace UI {
    namespace UIElements {
        class Text : public sf::Text, public Dockable {
            public:
                void Draw(sf::RenderWindow& window) override;
        };
    }
}