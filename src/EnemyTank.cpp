#include "EnemyTank.h"
#include <cmath>
#include <iostream>

// Constructor: tạo EnemyTank tại vị trí (x, y)
// Gọi constructor Enemy(x, y, 8) → máu 8 (trâu hơn Scout, yếu hơn Boss)
EnemyTank::EnemyTank(float x, float y) : Enemy(x, y, 8)
{
    // Nạp texture riêng cho Tank
    if (!tankTexture.loadFromFile("assets/Images/tank3.png"))
        std::cout << "Không thể tải tank2.png\n";
    else
    {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f); // Giữ kích thước vừa phải
    }

    // Đặt màu và kích thước hình chữ nhật body (làm hitbox)
    body.setFillColor(sf::Color(100, 100, 255)); // Màu xanh nhạt
    body.setSize(sf::Vector2f(30.f, 30.f));      // Có thể chỉnh to hơn nếu cần

    // Thông số đặc trưng của Tank
    shootCooldown = 2.5f;    // Bắn chậm hơn (so với Scout hoặc Boss)
    shootRange = 250.0f;     // Tầm bắn ngắn hơn
}

// Hàm update đơn giản → gọi bản update có tham số với dữ liệu trống
void EnemyTank::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    sf::Vector2f dummyPlayerPos(0.0f, 0.0f);
    update(deltaTime, emptyWalls, dummyPlayerPos);
}

// Hàm update có AI:
// - Tank di chuyển chậm nhưng máu trâu và chịu sát thương lâu
// - Dùng để truy đuổi hoặc giữ vị trí phòng thủ
void EnemyTank::update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos)
{
    speed = 40.f; // Chậm hơn Scout rất nhiều
    Enemy::update(deltaTime, walls, playerPos); // Gọi logic update chung
}
