// Bullet.cpp
#include "Bullet.h"
#include <cmath>

// Hàm khởi tạo viên đạn
Bullet::Bullet(const sf::Vector2f &startPos, const sf::Vector2f &dir, float spd)
	: direction(dir), speed(spd)
{
	// Tạo hình chữ nhật cơ bản cho đạn(khi không có texture)
	shape.setSize(sf::Vector2f(12.f, 12.f)); // hoặc 10.f, 10.f hoặc lớn hơn tuỳ ý
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(startPos);
	// skin đạn
	if (bulletTexture.loadFromFile("assets/Images/bullet1.png"))
	{
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setOrigin(bulletTexture.getSize().x / 2.f, bulletTexture.getSize().y / 2.f);
		bulletSprite.setScale(0.6f, 0.6f);  // Thu nhỏ sprite đạn
	}
}
// Cập nhật vị trí và góc quay của đạn
void Bullet::update(float dt)
{
	// Tính vector di chuyển = hướng * tốc độ * thời gian
	sf::Vector2f movement = direction * speed * dt;
	shape.move(movement); // đơn giản chỉ di chuyển đạn

	// Cập nhật vị trí sprite theo shape
	bulletSprite.setPosition(
		shape.getPosition().x + shape.getSize().x / 2,
		shape.getPosition().y + shape.getSize().y / 2);

	// Quay sprite theo hướng bay
	float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
	bulletSprite.setRotation(angle);
}
// Vẽ đạn ra cửa sổ
void Bullet::draw(sf::RenderWindow &window) const
{
	// Nếu có texture thì vẽ sprite, ngược lại vẽ hình chữ nhật
	if (bulletTexture.getSize().x > 0)
		window.draw(bulletSprite);
	else
		window.draw(shape);
}

// Kiểm tra đạn đã ra khỏi màn hình chưa
bool Bullet::isOffScreen(const sf::RenderWindow &window) const
{
	sf::Vector2u winSize = window.getSize(); // Lấy kích thước cửa sổ
	sf::Vector2f pos = shape.getPosition(); // Lấy vị trí đạn
	return (pos.x < 0 || pos.x > winSize.x || pos.y < 0 || pos.y > winSize.y);
}
// Lấy vùng bao quanh viên đạn (dùng để kiểm tra va chạm)
sf::FloatRect Bullet::getBounds() const
{
	return shape.getGlobalBounds();
}
// Lấy vị trí hiện tại của đạn
const sf::Vector2f &Bullet::getPosition() const
{
	return shape.getPosition();
}