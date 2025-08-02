// Wall.h
#pragma once
#include <SFML/Graphics.hpp>
class Wall {
public:
    Wall(const sf::Vector2f& pos, const sf::Vector2f& size, int durability = 3);

    void draw(sf::RenderWindow&) const;
    sf::FloatRect getBounds() const;
    bool isDestroyed() const;
    void takeDamage();

    void setTexture(const sf::Texture* tex);  // <-- Thêm

private:
    sf::RectangleShape shape;
    sf::Texture texture;
    int maxHit;
    int hitCount = 0;
};

