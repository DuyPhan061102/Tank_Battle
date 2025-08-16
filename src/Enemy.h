// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Wall.h"
#include "Bullet.h"

// Lớp Enemy kế thừa từ Tank, dùng làm lớp cha cho các loại kẻ địch
class Enemy : public Tank
{
public:
    // Constructor: khởi tạo Enemy tại vị trí (x, y) với máu ban đầu = health
    Enemy(float x, float y, int health);

    // Di chuyển Enemy theo hướng dx, dy (ghi đè từ Tank)
    void move(float dx, float dy) override;

    // Cập nhật trạng thái Enemy (ghi đè hàm thuần ảo từ Tank)
    void update(float deltaTime) override;

    // Hàm cập nhật có tham số tường và vị trí người chơi (cho AI đuổi/bắn)
    virtual void update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos);

    // Vẽ Enemy lên cửa sổ
    void draw(sf::RenderWindow& window) const override;

    // Setter tốc độ di chuyển
    void setSpeed(float newSpeed);

    // Kiểm tra xem Enemy có bị trúng đạn không (có thể override ở lớp con)
    virtual bool isHit(const sf::FloatRect& bounds);

    // Đánh dấu Enemy sẽ bị xóa ở frame tiếp theo
    void markToRemove();

    // Kiểm tra xem Enemy có cần bị xóa không
    bool shouldBeRemoved() const;

    // Enemy nhận sát thương
    void takeDamage(int amount);

    // Đuổi theo người chơi (AI cơ bản)
    void chasePlayer(const sf::Vector2f& playerPos, float dt, const std::vector<Wall>& walls);

    // Xác định có phải Boss không (mặc định là false, Boss override sẽ trả true)
    virtual bool isBoss() const { return false; }

    // Lấy máu tối đa
    int getMaxHP() const;

    // Hồi máu theo tỉ lệ (0.0f - 1.0f)
    void heal(float ratio);

    // Lấy máu hiện tại
    int getHP() const;

    // ================== AI bắn đạn ==================

    // Kiểm tra có tầm nhìn thẳng tới người chơi không (không bị tường che)
    bool hasLineOfSight(const sf::Vector2f& playerPos, const std::vector<Wall>& walls) const;

    // Kiểm tra có thể bắn người chơi không (phạm vi + tầm nhìn)
    bool canShoot(const sf::Vector2f& playerPos, const std::vector<Wall>& walls) const;

    // Bắn đạn "thông minh" (dựa vào tầm nhìn và khoảng cách)
    void smartShoot(const sf::Vector2f& playerPos, const std::vector<Wall>& walls);

    // ================== Quản lý đạn ==================

    // Lấy danh sách đạn (tham chiếu cho phép chỉnh sửa)
    std::vector<Bullet>& getBullets() { return bullets; }

    // Lấy danh sách đạn (tham chiếu chỉ đọc)
    const std::vector<Bullet>& getBullets() const { return bullets; }

    // Gán con trỏ tới cửa sổ để dùng khi cần vẽ hoặc lấy thông tin
    void setWindow(sf::RenderWindow* window) { windowPtr = window; }

    // Lấy hình chữ nhật bao quanh Enemy (dùng cho va chạm)
    sf::FloatRect getBounds() const;

protected:
    // ================== Dữ liệu chung của Enemy ==================
    sf::Texture tankTexture;       // Texture của xe tăng
    sf::Sprite tankSprite;         // Sprite hiển thị xe tăng
    sf::Vector2f direction;        // Hướng di chuyển hiện tại
    float timeSinceDirectionChange; // Thời gian từ lần đổi hướng gần nhất
    bool isHitEffect = false;      // Đang hiển thị hiệu ứng trúng đạn
    bool toBeRemoved = false;      // Đã đánh dấu để xóa
    sf::Clock hitClock;             // Đồng hồ đo thời gian hiệu ứng trúng đạn

    // ================== Hiệu ứng nổ ==================
    sf::Texture explosionTexture;  // Texture vụ nổ
    sf::Sprite explosionSprite;    // Sprite vụ nổ
    bool isExploding = false;      // Đang phát nổ
    float explosionTimer = 0.f;    // Bộ đếm thời gian phát nổ

    // ================== Chỉ số sinh tồn ==================
    int hp;                        // Máu hiện tại
    int maxHp;                     // Máu tối đa
    bool isDead = false;           // Đã chết chưa

    // ================== Hệ thống bắn đạn ==================
    std::vector<Bullet> bullets;   // Danh sách đạn đang tồn tại
    sf::Clock shootClock;          // Đồng hồ để giới hạn tốc độ bắn
    float shootCooldown = 2.0f;    // Thời gian chờ giữa 2 phát bắn
    float shootRange = 300.0f;     // Tầm bắn tối đa

private:
    sf::RenderWindow* windowPtr = nullptr; // Con trỏ tới cửa sổ render
};

#endif
