// Tank.cpp
#include "Tank.h"

// Constructor: khởi tạo hình dạng và thông số mặc định cho xe tăng
Tank::Tank()
{
    body.setSize(sf::Vector2f(40.f, 40.f));     // Kích thước xe tăng (40x40 pixels)
    body.setFillColor(sf::Color::White);        // Màu mặc định: trắng
    body.setPosition(100.f, 100.f);              // Vị trí khởi tạo trên màn hình
    speed = 100.f;                               // Tốc độ di chuyển mặc định
}

// Vẽ xe tăng lên cửa sổ
void Tank::draw(sf::RenderWindow& window) const {
    window.draw(body);
}

// Lấy vị trí hiện tại của xe tăng
sf::Vector2f Tank::getPosition() const {
    return body.getPosition();
}

// Đặt lại vị trí cho xe tăng
void Tank::setPosition(const sf::Vector2f& pos)
{
    body.setPosition(pos);
}

// Lấy khung bao (bounding box) của xe tăng để kiểm tra va chạm
sf::FloatRect Tank::getBounds() const {
    return body.getGlobalBounds();
}

// Lấy tốc độ hiện tại của xe tăng
float Tank::getSpeed() const {
    return speed;
}
