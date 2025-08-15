// Wall.h
#pragma once
#include <SFML/Graphics.hpp>

class Wall {
public:
    // Constructor:
    //  pos  -> vị trí trên màn hình
    //  size -> kích thước bức tường
    //  durability -> số lần trúng đạn trước khi phá hủy (mặc định 3)
    Wall(const sf::Vector2f& pos, const sf::Vector2f& size, int durability = 3);

    // Vẽ bức tường lên màn hình
    void draw(sf::RenderWindow&) const;

    // Lấy khung bao (bounding box) của tường (dùng để kiểm tra va chạm)
    sf::FloatRect getBounds() const;

    // Kiểm tra tường đã bị phá hủy chưa
    bool isDestroyed() const;

    // Gây sát thương cho tường (mỗi lần gọi, hitCount sẽ tăng)
    void takeDamage();

    // Gán texture cho tường từ bên ngoài (nếu muốn tường có hình ảnh)
    void setTexture(const sf::Texture* tex);

private:
    sf::RectangleShape shape;       // Hình dạng của tường
    const sf::Texture* texture = nullptr; // Con trỏ trỏ đến texture (nếu có)
    int maxHit;                     // Độ bền tối đa (số lần chịu đạn)
    int hitCount = 0;                // Số lần tường đã bị bắn
};
