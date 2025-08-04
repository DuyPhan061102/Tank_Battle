// Enemy.cpp
#include "Enemy.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

Enemy::Enemy(float x, float y)
{
    body.setPosition(x, y);
    body.setSize(sf::Vector2f(30.f, 30.f)); // hoặc 24.f, 24.f
    body.setFillColor(sf::Color::Red);
    speed = 50.f;
    timeSinceDirectionChange = 0.f;
    isHitEffect = false;
    toBeRemoved = false;

    // Load ảnh tank enemy
    if (!tankTexture.loadFromFile("assets/Images/tank2.png"))
        std::cout << "❌ Không thể tải enemy_tank.png\n";
    else {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f); // to hơn player một chút
    }

    int dir = rand() % 4;
    if (dir == 0)
        direction = {1.f, 0.f};
    else if (dir == 1)
        direction = {-1.f, 0.f};
    else if (dir == 2)
        direction = {0.f, 1.f};
    else
        direction = {0.f, -1.f};
}

void Enemy::update(float deltaTime)
{
    if (isExploding) {
        explosionTimer += deltaTime;
        if (explosionTimer > 0.4f) { // hiệu ứng nổ 0.4 giây
            toBeRemoved = true;
        }
        return; // Không di chuyển nữa khi đang nổ
    }

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = body.getSize();
    sf::Vector2f newPos = pos + direction * speed * deltaTime;

    if (newPos.x < 0 || newPos.x + size.x > 800)
        direction.x = -direction.x;
    if (newPos.y < 0 || newPos.y + size.y > 600)
        direction.y = -direction.y;

    body.move(direction * speed * deltaTime);

    // Cập nhật vị trí sprite theo body
    tankSprite.setPosition(body.getPosition().x + 15, body.getPosition().y + 15); // nửa size body

    // Thêm đoạn này để quay sprite theo hướng di chuyển
    if (direction.x != 0.f || direction.y != 0.f) {
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
        tankSprite.setRotation(angle);
    }

    timeSinceDirectionChange += deltaTime;

    if (timeSinceDirectionChange > 2.f)
    {
        int dir = rand() % 4;
        if (dir == 0)
            direction = {1.f, 0.f};
        else if (dir == 1)
            direction = {-1.f, 0.f};
        else if (dir == 2)
            direction = {0.f, 1.f};
        else
            direction = {0.f, -1.f};
        timeSinceDirectionChange = 0.f;
    }

    // ⏱ Sau khi trúng đạn thì chờ 0.2s rồi đánh dấu xoá
    if (isHitEffect && hitClock.getElapsedTime().asSeconds() > 0.2f)
    {
        toBeRemoved = true;
    }

    std::cout << "Enemy body pos: " << body.getPosition().x << ", " << body.getPosition().y << std::endl;
    std::cout << "Enemy sprite pos: " << tankSprite.getPosition().x << ", " << tankSprite.getPosition().y << std::endl;
}
void Enemy::draw(sf::RenderWindow &window) const
{
    if (isExploding)
        window.draw(explosionSprite);
    else if (tankTexture.getSize().x > 0)
        window.draw(tankSprite);
    else
        window.draw(body);
}

bool Enemy::isHit(const sf::FloatRect &bounds)
{
    if (!isHitEffect && body.getGlobalBounds().intersects(bounds))
    {
        isHitEffect = true;
        body.setFillColor(sf::Color::White); // Hiệu ứng khi trúng đạn
        hitClock.restart();
        return true;
    }
    return false;
}

void Enemy::markToRemove()
{
    if (!explosionTexture.loadFromFile("assets/Images/explosion.png")) {
        std::cout << "❌ Không thể tải explosion.png\n";
    } else {
        explosionSprite.setTexture(explosionTexture);
        explosionSprite.setOrigin(explosionTexture.getSize().x / 2.f, explosionTexture.getSize().y / 2.f);
        explosionSprite.setPosition(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
        isExploding = true;
        explosionTimer = 0.f;
    }
    toBeRemoved = false; // Đợi hiệu ứng xong mới xóa
}

bool Enemy::shouldBeRemoved() const
{
    return toBeRemoved;
}
void Enemy::move(float dx, float dy)
{
    body.move(dx, dy);
}
void Enemy::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

