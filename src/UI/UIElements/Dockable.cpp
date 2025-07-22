#include "Dockable.hpp"

namespace UI {
    namespace UIElements {
        void Dockable::setDockedPosition(float x, float y) {
            dockedPosition = {x, y};
        }

        sf::Vector2f Dockable::getDockedPosition() const {
            return dockedPosition;
        }

        void Dockable::setDock(Dock newDock) {
            dock = newDock;
        }

        Dock Dockable::getDock() const {
            return dock;
        }

        void Dockable::Draw(sf::RenderWindow& window) {
            // Default no-op; override in subclasses
        }
    } // namespace UIElements
} // namespace UI