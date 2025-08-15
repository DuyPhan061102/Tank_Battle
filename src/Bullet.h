// bullet.h
#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet
{
private:
	sf::RectangleShape shape; // Hình dạng cơ bản của viên đạn (nếu không dùng sprite)
	sf::Vector2f direction; // Hướng bay của đạn (vector đơn vị)
	float speed; 
	// skin đạn
	sf::Texture bulletTexture; // Texture đạn
	sf::Sprite bulletSprite; // Sprite hiển thị đạn


public:
	// Hàm khởi tạo viên đạn
	// startPos: vị trí bắt đầu
	// dir: hướng bay
	// speed: tốc độ bay (mặc định 100.f)
	Bullet(const sf::Vector2f &startPos, const sf::Vector2f &dir, float speed = 100.f);
	// Cập nhật vị trí đạn theo thời gian
	void update(float dt);
	// Vẽ đạn ra màn hình
	void draw(sf::RenderWindow &window) const;
	// Kiểm tra xem đạn đã bay ra ngoài màn hình chưa
	bool isOffScreen(const sf::RenderWindow &window) const;
	// Lấy hình chữ nhật bao quanh viên đạn (phục vụ va chạm)
	sf::FloatRect getBounds() const;
	// Lấy vị trí hiện tại của viên đạn
	const sf::Vector2f &getPosition() const;

};

#endif