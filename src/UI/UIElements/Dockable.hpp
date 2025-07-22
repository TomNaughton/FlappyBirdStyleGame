#pragma once
#include <SFML/Graphics.hpp>

namespace UI {
    namespace UIElements {
        enum Dock {
            DOCK_TOPLEFT,
            DOCK_TOPRIGHT,
            DOCK_BOTTOMLEFT,
            DOCK_BOTTOMRIGHT,
            DOCK_CENTER
        };
        
        class Dockable {
            public:
                Dockable() = default;

                void setDockedPosition(float x, float y);
                void setDock(Dock newDock);

                sf::Vector2f getDockedPosition() const;
                Dock getDock() const;

                virtual void Draw(sf::RenderWindow& window);

                ~Dockable() = default;

            private:
                Dock dock = DOCK_TOPLEFT;
                sf::Vector2f dockedPosition = {0, 0};
        };
    }
}