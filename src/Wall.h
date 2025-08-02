// Wall.h
#pragma once
#include <SFML/Graphics.hpp>

class Wall {
public:
    Wall(const sf::Vector2f& pos, const sf::Vector2f& size, int durability = 3);  // ✅ Sửa chỗ này

    void draw(sf::RenderWindow&) const;
    sf::FloatRect getBounds() const;
    bool isDestroyed() const;
    void takeDamage();

private:
    sf::RectangleShape shape;
    int maxHit;  // độ bền
    int hitCount = 0;
};
