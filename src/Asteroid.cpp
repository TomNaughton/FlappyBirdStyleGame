// #include "Asteroid.hpp"

// Asteroid::Asteroid(const std::vector<sf::Vector2f>& pts, const sf::Vector2f& pos)
//     : points(pts), position(pos), velocity(0.f, 0.f), rotation(0.f), angularVelocity(0.f)
// {
//     shape.setPointCount(points.size());
//     for (size_t i = 0; i < points.size(); ++i) {
//         shape.setPoint(i, points[i]);
//     }
//     shape.setOrigin(0.f, 0.f);
//     shape.setPosition(position);
//     shape.setFillColor(sf::Color::Transparent);
//     shape.setOutlineColor(sf::Color::White);
//     shape.setOutlineThickness(1.f);
// }

// void Asteroid::draw(sf::RenderWindow& window) const {
//     window.draw(shape);
// }

// void Asteroid::update(float dt) {
//     position += velocity * dt;
//     rotation += angularVelocity * dt;
//     shape.setPosition(position);
//     shape.setRotation(rotation);
// }

// bool Asteroid::pointInPolygon(const sf::Vector2f& p) const {
//     bool inside = false;
//     int count = shape.getPointCount();
//     for (int i = 0, j = count - 1; i < count; j = i++) {
//         sf::Vector2f pi = shape.getTransform().transformPoint(shape.getPoint(i));
//         sf::Vector2f pj = shape.getTransform().transformPoint(shape.getPoint(j));
//         if (((pi.y > p.y) != (pj.y > p.y)) &&
//             (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y) + pi.x)) {
//             inside = !inside;
//         }
//     }
//     return inside;
// }

// bool Asteroid::containsPoint(const sf::Vector2f& point) const {
//     return pointInPolygon(point);
// }

// #include <optional>

// std::optional<sf::Vector2f> lineSegmentIntersection(
//     sf::Vector2f p, sf::Vector2f p2, sf::Vector2f q, sf::Vector2f q2)
// {
//     sf::Vector2f r = p2 - p;
//     sf::Vector2f s = q2 - q;
//     float rxs = r.x * s.y - r.y * s.x;
//     if (rxs == 0) return std::nullopt; // parallel

//     sf::Vector2f qp = q - p;
//     float t = (qp.x * s.y - qp.y * s.x) / rxs;
//     float u = (qp.x * r.y - qp.y * r.x) / rxs;

//     if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
//         return p + t * r;
//     }
//     return std::nullopt;
// }

// std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>
// splitPolygon(
//     const std::vector<sf::Vector2f>& polygon,
//     const sf::Vector2f& lineP1,
//     const sf::Vector2f& lineP2
// ) {
//     std::vector<sf::Vector2f> leftPoly;
//     std::vector<sf::Vector2f> rightPoly;

//     size_t n = polygon.size();

//     for (size_t i = 0; i < n; ++i) {
//         const sf::Vector2f& current = polygon[i];
//         const sf::Vector2f& next = polygon[(i + 1) % n];

//         float sideCurrent = sideOfLine(lineP1, lineP2, current);
//         float sideNext = sideOfLine(lineP1, lineP2, next);

//         // Add current vertex to corresponding polygon(s)
//         if (sideCurrent >= 0)
//             leftPoly.push_back(current);
//         if (sideCurrent <= 0)
//             rightPoly.push_back(current);

//         // Edge crosses the line: find intersection
//         if ((sideCurrent > 0 && sideNext < 0) || (sideCurrent < 0 && sideNext > 0)) {
//             auto intersectOpt = lineSegmentIntersection(current, next, lineP1, lineP2);
//             if (intersectOpt) {
//                 sf::Vector2f intersect = *intersectOpt;
//                 leftPoly.push_back(intersect);
//                 rightPoly.push_back(intersect);
//             }
//         }
//     }

//     return {leftPoly, rightPoly};
// }




// std::vector<Asteroid> Asteroid::split(const sf::Vector2f& lineP1, const sf::Vector2f& lineP2) const {
//     auto [leftPoly, rightPoly] = splitPolygon(points, lineP1, lineP2);

//     std::vector<Asteroid> pieces;
//     if (leftPoly.size() >= 3)
//         pieces.emplace_back(leftPoly, position);
//     if (rightPoly.size() >= 3)
//         pieces.emplace_back(rightPoly, position);

//     return pieces;
// }


// float sideOfLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& p) {
//     // >0: left, <0: right, 0: on line
//     return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
// }

// // Helper: compute cross product (2D)
// float cross(const sf::Vector2f& a, const sf::Vector2f& b) {
//     return a.x * b.y - a.y * b.x;
// }

// // Helper: compute intersection of line segments p1-p2 and p3-p4
// // Assumes they intersect
// sf::Vector2f lineIntersection(const sf::Vector2f& p1, const sf::Vector2f& p2,
//                              const sf::Vector2f& p3, const sf::Vector2f& p4) {
//     sf::Vector2f s1 = p2 - p1;
//     sf::Vector2f s2 = p4 - p3;
//     float s = (-s1.y * (p1.x - p3.x) + s1.x * (p1.y - p3.y)) / (-s2.x * s1.y + s1.x * s2.y);
//     return p3 + s * s2;
// }

// // Clip polygon to one side of infinite line defined by lineP1->lineP2
// std::vector<sf::Vector2f> clipPolygonToHalfPlane(const std::vector<sf::Vector2f>& poly,
//     const sf::Vector2f& lineP1, const sf::Vector2f& lineP2, bool keepLeftSide) {

//     std::vector<sf::Vector2f> output;

//     int n = poly.size();
//     for (int i = 0; i < n; ++i) {
//         const sf::Vector2f& current = poly[i];
//         const sf::Vector2f& prev = poly[(i + n - 1) % n];

//         // Determine which side points lie on:
//         // Compute signed distance of point to line
//         // positive if on left side (looking from lineP1->lineP2)
//         auto side = [&](const sf::Vector2f& p) -> float {
//             return cross(lineP2 - lineP1, p - lineP1);
//         };

//         float currSide = side(current);
//         float prevSide = side(prev);

//         bool currInside = keepLeftSide ? (currSide >= 0) : (currSide <= 0);
//         bool prevInside = keepLeftSide ? (prevSide >= 0) : (prevSide <= 0);

//         if (currInside) {
//             if (!prevInside) {
//                 // Edge from outside to inside, add intersection
//                 sf::Vector2f inter = lineIntersection(prev, current, lineP1, lineP2);
//                 output.push_back(inter);
//             }
//             output.push_back(current);
//         }
//         else if (prevInside) {
//             // Edge from inside to outside, add intersection
//             sf::Vector2f inter = lineIntersection(prev, current, lineP1, lineP2);
//             output.push_back(inter);
//         }
//         // else both outside: add nothing
//     }

//     return output;
// }

// std::vector<Asteroid> Asteroid::split(const sf::Vector2f& lineP1, const sf::Vector2f& lineP2) const {
//     // Clip polygon to left side of line
//     std::vector<sf::Vector2f> leftPoly = clipPolygonToHalfPlane(points, lineP1, lineP2, true);
//     // Clip polygon to right side of line
//     std::vector<sf::Vector2f> rightPoly = clipPolygonToHalfPlane(points, lineP1, lineP2, false);

//     std::vector<Asteroid> pieces;

//     // Only keep polygons with at least 3 points
//     if (leftPoly.size() >= 3) {
//         Asteroid leftAsteroid = *this;
//         leftAsteroid.points = std::move(leftPoly);
//         pieces.push_back(std::move(leftAsteroid));
//     }
//     if (rightPoly.size() >= 3) {
//         Asteroid rightAsteroid = *this;
//         rightAsteroid.points = std::move(rightPoly);
//         pieces.push_back(std::move(rightAsteroid));
//     }

//     return pieces;
// }

// void handleProjectileCollision(const Projectile& projectile, Asteroid& asteroid) {
//     if (!pointInPolygon(asteroid.points, projectile.position)) return;

//     sf::Vector2f impactPoint = projectile.position;
//     sf::Vector2f projectileDir = normalize(projectile.velocity);

//     // Perpendicular line through impact point
//     sf::Vector2f perp(-projectileDir.y, projectileDir.x);

//     sf::Vector2f lineP1 = impactPoint + perp * 1000.f;
//     sf::Vector2f lineP2 = impactPoint - perp * 1000.f;

//     auto newAsteroids = asteroid.split(lineP1, lineP2);

//     // Remove original asteroid from your asteroid list and add new ones
//     removeAsteroid(asteroid);
//     for (auto& a : newAsteroids)
//         addAsteroid(std::move(a));
// }

// bool pointInPolygon(const std::vector<sf::Vector2f>& polygon, const sf::Vector2f& point) {
//     int n = polygon.size();
//     for (int i = 0; i < n; ++i) {
//         sf::Vector2f a = polygon[i];
//         sf::Vector2f b = polygon[(i + 1) % n];
//         if (cross(b - a, point - a) < 0) {
//             return false;
//         }
//     }
//     return true;
// }
