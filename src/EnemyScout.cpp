#include "EnemyScout.h"
#include <iostream>

// Constructor: tạo một EnemyScout tại vị trí (x, y)
// Gọi constructor của Enemy (x, y, 3) → máu chỉ 3 (yếu hơn boss và tank)
EnemyScout::EnemyScout(float x, float y) : Enemy(x, y, 3)
{
    // Nạp texture riêng cho Scout
    if (!tankTexture.loadFromFile("assets/Images/tank4.png"))
        std::cout << "Không thể tải scout.png\n";
    else
    {
        tankSprite.setTexture(tankTexture);
        // Đặt tâm sprite ở giữa để xoay chính xác
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f); // Scout nhỏ gọn, nhanh nhẹn hơn
    }

    // Đổi màu hình chữ nhật body để phân biệt
    body.setFillColor(sf::Color::Yellow);
}

// Hàm update không cần AI phức tạp → gọi bản update có tham số với dữ liệu trống
void EnemyScout::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    sf::Vector2f dummyPlayerPos(0.0f, 0.0f);
    update(deltaTime, emptyWalls, dummyPlayerPos);
}

// Hàm update với AI: Scout có tốc độ cao để truy đuổi hoặc né tránh
// walls: danh sách tường để tránh va chạm
// playerPos: vị trí người chơi (dùng để tracking)
void EnemyScout::update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos)
{
    speed = 90.f; // Nhanh nhất trong các loại enemy
    Enemy::update(deltaTime, walls, playerPos); // Gọi logic update chung
}
