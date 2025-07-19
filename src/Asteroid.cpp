#include "Asteroid.hpp"
#include <cmath>
#include <iostream>
#include "clipper.hpp"

using namespace ClipperLib;

Asteroid::Asteroid(const std::vector<sf::Vector2f>& poly, sf::Vector2f pos, sf::Vector2f vel, float rotSpeed)
    : polygon(poly), position(pos), velocity(vel), rotationSpeed(rotSpeed), rotation(0.f) {}

void Asteroid::update(float dt) {
    position += velocity * dt;
    rotation += rotationSpeed * dt;
    if (rotation > 360.f) rotation -= 360.f;
    if (rotation < 0.f) rotation += 360.f;
}

void Asteroid::draw(sf::RenderWindow& window) const {
    sf::ConvexShape shape;
    shape.setPointCount(polygon.size());
    float rad = rotation * 3.14159265f / 180.f;

    for (size_t i = 0; i < polygon.size(); ++i) {
        sf::Vector2f rotated(
            polygon[i].x * std::cos(rad) - polygon[i].y * std::sin(rad),
            polygon[i].x * std::sin(rad) + polygon[i].y * std::cos(rad)
        );
        shape.setPoint(i, rotated);
    }
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.f);
    window.draw(shape);
}

bool Asteroid::containsPoint(const sf::Vector2f& point) const {
    // Convert global point to local asteroid space
    sf::Vector2f localPoint = point - position;

    int crossings = 0;
    size_t n = polygon.size();
    for (size_t i = 0; i < n; ++i) {
        const sf::Vector2f& a = polygon[i];
        const sf::Vector2f& b = polygon[(i + 1) % n];
        if (((a.y > localPoint.y) != (b.y > localPoint.y)) &&
            (localPoint.x < (b.x - a.x) * (localPoint.y - a.y) / ((b.y - a.y) + 0.00001f) + a.x)) {
            crossings++;
        }
    }
    return (crossings % 2) == 1;
}

sf::Vector2f Asteroid::getPosition() const {
    return position;
}

sf::Vector2f Asteroid::getVelocity() const {
    return velocity;
}

std::vector<Asteroid> Asteroid::splitAt(const sf::Vector2f& impactPoint, const sf::Vector2f& projectileVelocity) {
    std::vector<Asteroid> result;

    const double scale = 1000.0;

    // Convert polygon to Clipper Path in world coordinates (local polygon + position)
    Path subj;
    for (const auto& pt : polygon) {
        subj.push_back(IntPoint(static_cast<cInt>((pt.x + position.x) * scale), static_cast<cInt>((pt.y + position.y) * scale)));
    }

    // Create circular cutter polygon centered on impactPoint
    const int cutSegments = 20;
    const float cutRadius = 10.f;
    Path cutter;
    for (int i = 0; i < cutSegments; ++i) {
        float angle = i * 2.f * 3.14159265f / cutSegments;
        float x = impactPoint.x + cutRadius * std::cos(angle);
        float y = impactPoint.y + cutRadius * std::sin(angle);
        cutter.push_back(IntPoint(static_cast<cInt>(x * scale), static_cast<cInt>(y * scale)));
    }

    // Perform clipping: difference (remaining asteroid) and intersection (cut fragment)
    Paths remainingPaths;
    Clipper clipper1;
    clipper1.AddPath(subj, ptSubject, true);
    clipper1.AddPath(cutter, ptClip, true);
    clipper1.Execute(ctDifference, remainingPaths, pftNonZero, pftNonZero);

    Paths removedPaths;
    Clipper clipper2;
    clipper2.AddPath(subj, ptSubject, true);
    clipper2.AddPath(cutter, ptClip, true);
    clipper2.Execute(ctIntersection, removedPaths, pftNonZero, pftNonZero);

    // Normalize projectile velocity direction (avoid zero length)
    sf::Vector2f projDir = projectileVelocity;
    float len = std::sqrt(projDir.x * projDir.x + projDir.y * projDir.y);
    if (len > 0.0001f) {
        projDir /= len;
    } else {
        projDir = sf::Vector2f(0.f, 0.f);
    }

    float recoilSpeed = 50.f; // Adjust recoil strength as desired

    // Create new Asteroids for remaining parts (recoil opposite projectile)
    for (const auto& path : remainingPaths) {
        if (path.size() < 3) continue;

        std::vector<sf::Vector2f> newPoly;
        for (const auto& pt : path) {
            newPoly.emplace_back(pt.X / scale - position.x, pt.Y / scale - position.y);
        }

        sf::Vector2f newVel = (velocity - projDir * recoilSpeed) * 0.1f;

        result.emplace_back(newPoly, position, -newVel, rotationSpeed);
    }

    // Create new Asteroids for removed parts (fragments, fly roughly with projectile)
    for (const auto& path : removedPaths) {
        if (path.size() < 3) continue;

        std::vector<sf::Vector2f> fragPoly;
        sf::Vector2f fragCenter(0.f, 0.f);

        for (const auto& pt : path) {
            sf::Vector2f ptf(pt.X / scale, pt.Y / scale);
            fragPoly.push_back(ptf);
            fragCenter += ptf;
        }
        fragCenter /= static_cast<float>(fragPoly.size());

        // Re-center polygon around centroid
        for (auto& pt : fragPoly) {
            pt -= fragCenter;
        }

        // Fragment position in world coords
        sf::Vector2f fragPos = fragCenter;

        // Fragment velocity = original velocity plus projectile direction scaled
        sf::Vector2f fragVel = velocity + projDir * recoilSpeed * 1.5f;
        
        result.emplace_back(fragPoly, fragPos, -fragVel, rotationSpeed);
    }

    return result;
}
