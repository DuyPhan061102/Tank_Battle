#include "Wall.h"

// Constructor
// pos: vị trí đặt bức tường
// size: kích thước của bức tường
// durability: số lần tường chịu được sát thương trước khi bị phá hủy
Wall::Wall(const sf::Vector2f& pos, const sf::Vector2f& size, int durability)
    : maxHit(durability), hitCount(0) // gán độ bền tối đa và số lần bị bắn ban đầu
{
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(sf::Color(150, 150, 150)); // màu xám mặc định (nếu không có texture)
}

// Vẽ bức tường
void Wall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Lấy khung va chạm của tường
sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}

// Gây sát thương cho tường
void Wall::takeDamage() {
    hitCount++; // mỗi lần gọi là tường mất 1 máu
}

// Kiểm tra xem tường đã bị phá hủy chưa
bool Wall::isDestroyed() const {
    return hitCount >= maxHit;
}

// Gán texture cho tường
void Wall::setTexture(const sf::Texture* tex) {
    texture = tex; // lưu lại con trỏ texture
    if (texture)
        shape.setTexture(texture); // áp dụng texture vào shape nếu có
}
