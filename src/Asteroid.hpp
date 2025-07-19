// #pragma once
// #include <SFML/Graphics.hpp>
// #include <vector>

// class Asteroid {
// public:
//     Asteroid(const std::vector<sf::Vector2f>& points, const sf::Vector2f& position);

//     void update(float dt);
//     void draw(sf::RenderWindow& window) const;

//     // Checks if a point hits the asteroid polygon (simple point-in-polygon test)
//     bool containsPoint(const sf::Vector2f& point) const;

//     // Split asteroid polygon along a line (defined by two points)
//     // Returns a vector of new Asteroids created by splitting this one
//     std::vector<Asteroid> split(const sf::Vector2f& lineP1, const sf::Vector2f& lineP2) const;

//     sf::Vector2f getPosition() const { return position; }
//     void setVelocity(const sf::Vector2f& vel) { velocity = vel; }

// private:
//     sf::ConvexShape shape;
//     std::vector<sf::Vector2f> points; // polygon vertices local to position
//     sf::Vector2f position;
//     sf::Vector2f velocity;
//     float rotation = 0.f;
//     float angularVelocity = 0.f;

//     // Helper: point-in-polygon test
//     bool pointInPolygon(const sf::Vector2f& point) const;

//     // Helper: split polygon implementation (math heavy)
//     std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>> splitPolygon(
//         const std::vector<sf::Vector2f>& polygon,
//         const sf::Vector2f& p1,
//         const sf::Vector2f& p2
//     ) const;
// };
