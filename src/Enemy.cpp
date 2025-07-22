#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(float x, float y)
{
    body.setPosition(x, y);
    body.setSize(sf::Vector2f(40.f, 40.f));
    body.setFillColor(sf::Color::Red);
    speed = 50.f;
    timeSinceDirectionChange = 0.f;
    isHitEffect = false;
    toBeRemoved = false;

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
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = body.getSize();
    sf::Vector2f newPos = pos + direction * speed * deltaTime;

    if (newPos.x < 0 || newPos.x + size.x > 800)
        direction.x = -direction.x;
    if (newPos.y < 0 || newPos.y + size.y > 600)
        direction.y = -direction.y;

    body.move(direction * speed * deltaTime);
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
}
void Enemy::draw(sf::RenderWindow &window) const
{
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
    toBeRemoved = true;
}

bool Enemy::shouldBeRemoved() const
{
    return toBeRemoved;
}
void Enemy::move(float dx, float dy)
{
    body.move(dx, dy);
}
