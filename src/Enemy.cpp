// Enemy.cpp
#include "Enemy.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

Enemy::Enemy(float x, float y, int hpValue)
{
    body.setPosition(x, y);
    body.setSize(sf::Vector2f(30.f, 30.f));
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


void Enemy::update(float deltaTime, const std::vector<Wall>& walls)
{
    if (isExploding) {
        explosionTimer += deltaTime;
        if (explosionTimer > 0.4f) {
            toBeRemoved = true;
        }
        return;
    }

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = body.getSize();
    sf::Vector2f newPos = pos + direction * speed * deltaTime;

    // Tạo bounds mới cho enemy sau khi di chuyển
    sf::FloatRect futureBounds(newPos.x, newPos.y, size.x, size.y);

    bool collision = false;
    for (const Wall& wall : walls) {
        if (wall.getBounds().intersects(futureBounds)) {
            collision = true;
            break;
        }
    }

    // Kiểm tra biên map (window 800x600, enemy 40x40)
    if (newPos.x < 0 || newPos.x + size.x > 800 ||
        newPos.y < 0 || newPos.y + size.y > 600)
    {
        collision = true;
    }

    if (!collision) {
        body.move(direction * speed * deltaTime);
    } else {
        // Đổi hướng ngẫu nhiên khi va chạm tường hoặc biên
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

    // Cập nhật vị trí sprite theo body
    tankSprite.setPosition(body.getPosition());

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
        isHitEffect = false;
        body.setFillColor(sf::Color::Red);
    }

}

void Enemy::update(float deltaTime)
{
    // Gọi update với walls rỗng nếu không có walls
    std::vector<Wall> emptyWalls;
    update(deltaTime, emptyWalls);
}

void Enemy::draw(sf::RenderWindow &window) const
{
    if (isExploding)
        window.draw(explosionSprite);
    else if (tankTexture.getSize().x > 0)
        window.draw(tankSprite);
    else
        window.draw(body);

    // Vẽ thanh máu
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
    // Nếu là va chạm với đạn (không phải player), luôn trừ máu
    if (body.getGlobalBounds().intersects(bounds))
    {
        std::cout << "Enemy hit! HP: " << hp << std::endl;
        takeDamage(1);
        return true;
    }
    return false;
}

void Enemy::takeDamage(int amount)  
{   
    if (isDead) return;
    if (hp <= 0) return;
    hp -= amount;
    if (hp < 0) hp = 0;
    isHitEffect = true;
    body.setFillColor(sf::Color::White);
    hitClock.restart();

    if (hp == 0)
    {
        toBeRemoved = true;
        isDead = true;
    }
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

void Enemy::chasePlayer(const sf::Vector2f& playerPos, float dt, const std::vector<Wall>& walls)
{
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f dir = playerPos - pos;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f)
        dir /= len;

    sf::Vector2f movement = dir * speed * dt;
    sf::FloatRect futureBounds = body.getGlobalBounds();
    futureBounds.left += movement.x;
    futureBounds.top += movement.y;

    bool collision = false;
    for (const Wall& wall : walls) {
        if (wall.getBounds().intersects(futureBounds)) {
            collision = true;
            break;
        }
    }

    // Kiểm tra biên map
    if (futureBounds.left < 0 || futureBounds.left + futureBounds.width > 800 ||
        futureBounds.top < 0 || futureBounds.top + futureBounds.height > 600)
    {
        collision = true;
    }

    if (!collision)
        body.move(movement);
    // Nếu va chạm thì không di chuyển hoặc có thể đổi hướng ngẫu nhiên
}

int Enemy::getMaxHP() const {
    return maxHp;
}

void Enemy::heal(float ratio) {
    int amount = static_cast<int>(maxHp * ratio);
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}
int Enemy::getHP() const {
    return hp;
}