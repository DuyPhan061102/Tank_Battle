#pragma once
#include "Enemy.h"

// Lớp EnemyTank kế thừa từ Enemy
// Đại diện cho loại kẻ địch dạng "xe tăng"
class EnemyTank : public Enemy {
public:
    // Constructor: khởi tạo EnemyTank tại vị trí (x, y)
    EnemyTank(float x, float y);

    // Cập nhật trạng thái EnemyTank mỗi frame
    // deltaTime: thời gian trôi qua giữa 2 frame
    void update(float deltaTime) override;

    // Cập nhật trạng thái EnemyTank khi có thông tin tường và vị trí người chơi
    // walls: danh sách tường để xử lý va chạm
    // playerPos: vị trí hiện tại của người chơi
    // → Cho phép EnemyTank di chuyển chiến thuật hoặc tấn công dựa trên vị trí người chơi
    void update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos) override;
};
