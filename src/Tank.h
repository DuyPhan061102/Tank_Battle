#pragma once
#include <SFML/Graphics.hpp>

// Lớp cơ sở trừu tượng cho tất cả các loại xe tăng (người chơi hoặc kẻ địch)
class Tank
{
protected:
    sf::RectangleShape body; // Hình dạng (thân) xe tăng
    float speed;             // Tốc độ di chuyển của xe tăng

public:
    Tank();                  // Constructor mặc định
    virtual ~Tank() = default; // Destructor ảo để đảm bảo hủy đúng kiểu khi kế thừa

    // Hàm cập nhật trạng thái xe tăng (thuần ảo - lớp con phải định nghĩa)
    virtual void update(float deltaTime) = 0;

    // Hàm di chuyển xe tăng (thuần ảo - lớp con phải định nghĩa)
    virtual void move(float dx, float dy) = 0;

    // Vẽ xe tăng lên cửa sổ
    virtual void draw(sf::RenderWindow& window) const;

    // Đặt vị trí cho xe tăng
    void setPosition(const sf::Vector2f& pos);

    // Lấy vị trí hiện tại của xe tăng
    sf::Vector2f getPosition() const;

    // Lấy kích thước khung bao (bounding box) của xe tăng
    virtual sf::FloatRect getBounds() const;

    // Lấy tốc độ hiện tại của xe tăng
    float getSpeed() const;
};
