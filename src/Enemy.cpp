#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(float x, float y, int hpValue)
{
    body.setPosition(x, y);
    body.setSize(sf::Vector2f(40.f, 40.f));
    body.setFillColor(sf::Color::Red);
    speed = 50.f;
    timeSinceDirectionChange = 0.f;
    isHitEffect = false;
    toBeRemoved = false;

    hp = hpValue;
    maxHp = hpValue;

    int dir = rand() % 4;
    if (dir == 0)
        direction = { 1.f, 0.f };
    else if (dir == 1)
        direction = { -1.f, 0.f };
    else if (dir == 2)
        direction = { 0.f, 1.f };
    else
        direction = { 0.f, -1.f };
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
        isHitEffect = false;
        body.setFillColor(sf::Color::Red);
    }

}
void Enemy::draw(sf::RenderWindow &window) const
{
    window.draw(body);
    sf::RectangleShape healthBarBg(sf::Vector2f(40.f, 5.f));
    healthBarBg.setFillColor(sf::Color(50, 50, 50));
    healthBarBg.setPosition(body.getPosition().x, body.getPosition().y - 8);

    sf::RectangleShape healthBar(sf::Vector2f(40.f * ((float)hp / maxHp), 5.f));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(body.getPosition().x, body.getPosition().y - 8);

    window.draw(healthBarBg);
    window.draw(healthBar);
}

bool Enemy::isHit(const sf::FloatRect &bounds)
{
    if (!isHitEffect && body.getGlobalBounds().intersects(bounds))
    {
        takeDamage(1);  // ✅ GỌI HÀM MỚI
        return true;
    }
    return false;
}

void Enemy::takeDamage(int amount)  
{
    hp -= amount;
    isHitEffect = true;
    body.setFillColor(sf::Color::White);
    hitClock.restart();

    if (hp <= 0)
    {
        toBeRemoved = true;
    }
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
void Enemy::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Enemy::chasePlayer(const sf::Vector2f& playerPos, float dt)
{
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f dir = playerPos - pos;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f)
        dir /= len;

    body.move(dir * speed * dt);
}
