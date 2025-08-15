#pragma once
#include "Enemy.h"

// Lớp EnemyScout kế thừa từ Enemy
// Đại diện cho loại kẻ địch di chuyển nhanh, chuyên dò tìm hoặc bám theo người chơi
class EnemyScout : public Enemy
{
public:
    // Constructor: khởi tạo EnemyScout tại vị trí (x, y)
    EnemyScout(float x, float y);

    // Cập nhật trạng thái của EnemyScout (chạy mỗi frame)
    // deltaTime: thời gian trôi qua giữa 2 frame
    void update(float deltaTime) override;

    // Cập nhật trạng thái của EnemyScout khi có thông tin tường và vị trí người chơi
    // walls: danh sách tường để tránh va chạm
    // playerPos: vị trí hiện tại của người chơi
    // → Cho phép EnemyScout di chuyển theo hướng người chơi hoặc tránh vật cản
    void update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos) override;
};
