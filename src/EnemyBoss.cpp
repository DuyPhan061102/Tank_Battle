#include "EnemyBoss.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Constructor: tạo một EnemyBoss tại vị trí (x, y)
// Gọi constructor của Enemy (x, y, 30) → boss có máu ban đầu là 30
EnemyBoss::EnemyBoss(float x, float y) : Enemy(x, y, 30)
{
    // Kích thước hình chữ nhật đại diện cho boss (lớn hơn enemy thường)
    body.setSize(sf::Vector2f(60.f, 60.f));
    body.setFillColor(sf::Color::Red); // Màu đỏ để phân biệt

    // Nạp texture riêng cho Boss
    if (!tankTexture.loadFromFile("assets/Images/tank5.png"))
        std::cout << "Không thể tải boss.png\n";
    else
    {
        tankSprite.setTexture(tankTexture);
        // Đặt tâm sprite ở giữa để xoay và di chuyển chính xác
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.7f, 0.7f); // Boss to hơn bình thường
    }

    // Đặt vị trí ban đầu
    body.setPosition(x, y);

    // Thay đổi đặc tính bắn của boss
    shootCooldown = 1.0f; // Bắn nhanh hơn enemy thường
    shootRange = 400.0f;  // Tầm bắn xa hơn
}

// Hàm update không dùng AI phức tạp (không có tường, không tracking player)
// → Gọi phiên bản update có tham số với dữ liệu trống
void EnemyBoss::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    sf::Vector2f dummyPlayerPos(0.0f, 0.0f);
    update(deltaTime, emptyWalls, dummyPlayerPos);
}

// Hàm update với AI: cho Boss di chuyển và xử lý va chạm dựa trên vị trí player và tường
void EnemyBoss::update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos)
{
    speed = 25.f; // Boss di chuyển chậm hơn scout nhưng trâu hơn
    Enemy::update(deltaTime, walls, playerPos); // Gọi logic update chung từ Enemy
}

// Kiểm tra xem Boss có bị trúng đạn không
// bounds: vùng va chạm của viên đạn
bool EnemyBoss::isHit(const sf::FloatRect& bounds)
{
    if (getBounds().intersects(bounds)) // Nếu vùng va chạm trùng nhau
    {
        takeDamage(1); // Boss mất 1 máu
        return true;   // Trả về true → đã bị trúng
    }
    return false; // Không bị trúng
}
