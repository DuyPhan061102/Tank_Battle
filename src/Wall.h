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
    void setTexture(const sf::Texture* tex);  // Gán texture từ bên ngoài

private:
    sf::RectangleShape shape;
    const sf::Texture* texture = nullptr;  // 👍 Chỉ giữ con trỏ, không lưu ảnh
    int maxHit;
    int hitCount = 0;
};


