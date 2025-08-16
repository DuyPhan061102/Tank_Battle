#pragma once
#include <iostream>
#include "Enemy.h"

// Lớp EnemyBoss kế thừa từ Enemy
// Đại diện cho loại kẻ địch đặc biệt (trùm/boss) trong game
class EnemyBoss : public Enemy
{
private:
    int hp; // Máu của boss (lớn hơn enemy thường)

public:
    // Constructor: khởi tạo boss tại vị trí (x, y)
    EnemyBoss(float x, float y);

    // Cập nhật trạng thái boss (ví dụ di chuyển, animation) mỗi frame
    // deltaTime: thời gian trôi qua giữa 2 frame
    void update(float deltaTime) override;

    // Cập nhật trạng thái boss với thông tin tường và vị trí người chơi
    // Giúp boss di chuyển thông minh hơn (AI) dựa vào bản đồ và vị trí người chơi
    void update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos) override;

    // Kiểm tra xem boss có bị trúng đạn hay không
    // bounds: vùng va chạm của viên đạn hoặc vật thể
    bool isHit(const sf::FloatRect& bounds) override;

    // Xác nhận đây là Boss (dùng để phân biệt với enemy thường)
    bool isBoss() const override { return true; }
};
