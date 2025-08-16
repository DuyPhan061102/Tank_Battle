// Enemy.cpp
#include "Enemy.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

// Constructor - khởi tạo enemy với vị trí, máu, tốc độ và hướng di chuyển ngẫu nhiên
Enemy::Enemy(float x, float y, int hpValue)
{
    body.setPosition(x, y); // Đặt vị trí
    body.setSize(sf::Vector2f(30.f, 30.f)); // Kích thước khối body (nếu không có texture)
    body.setFillColor(sf::Color::Red);
    speed = 50.f; // Tốc độ di chuyển
    timeSinceDirectionChange = 0.f; // Thời gian kể từ khi đổi hướng
    isHitEffect = false; // Hiệu ứng khi bị trúng đạn
    toBeRemoved = false; // Cờ đánh dấu xoá

    hp = hpValue;
    maxHp = hpValue;

    // Chọn hướng di chuyển ban đầu ngẫu nhiên
    int dir = rand() % 4;
    if (dir == 0) direction = { 1.f, 0.f };
    else if (dir == 1) direction = { -1.f, 0.f };
    else if (dir == 2) direction = { 0.f, 1.f };
    else direction = { 0.f, -1.f };
}

// Update có tham số walls và playerPos - xử lý di chuyển, va chạm, bắn đạn
void Enemy::update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos)
{
    // Nếu đang trong trạng thái nổ
    if (isExploding)
    {
        explosionTimer += deltaTime;
        if (explosionTimer > 0.4f) // Hết hiệu ứng nổ thì đánh dấu xoá
        {
            toBeRemoved = true;
        }
        return; // Không làm gì thêm khi đang nổ
    }

    // Tính vị trí mới dựa trên hướng và tốc độ
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = body.getSize();
    sf::Vector2f newPos = pos + direction * speed * deltaTime;

    // Tạo bounds giả để kiểm tra va chạm trước khi di chuyển
    sf::FloatRect futureBounds(newPos.x, newPos.y, size.x, size.y);

    bool collision = false;
    // Kiểm tra va chạm với tường
    for (const Wall& wall : walls)
    {
        if (wall.getBounds().intersects(futureBounds))
        {
            collision = true;
            break;
        }
    }

    // Kiểm tra va chạm biên bản đồ
    if (newPos.x < 0 || newPos.x + size.x > 800 ||
        newPos.y < 0 || newPos.y + size.y > 600)
    {
        collision = true;
    }

    if (!collision)
    {
        // Không va chạm → di chuyển bình thường
        body.move(direction * speed * deltaTime);
    }
    else
    {
        // Va chạm → đổi hướng ngẫu nhiên
        int dir = rand() % 4;
        if (dir == 0) direction = { 1.f, 0.f };
        else if (dir == 1) direction = { -1.f, 0.f };
        else if (dir == 2) direction = { 0.f, 1.f };
        else direction = { 0.f, -1.f };
        timeSinceDirectionChange = 0.f;
    }

    // Đồng bộ vị trí sprite với body
    tankSprite.setPosition(body.getPosition());

    // Quay sprite theo hướng di chuyển
    if (direction.x != 0.f || direction.y != 0.f)
    {
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
        tankSprite.setRotation(angle);
    }

    // Đổi hướng ngẫu nhiên mỗi 2 giây
    timeSinceDirectionChange += deltaTime;
    if (timeSinceDirectionChange > 2.f)
    {
        int dir = rand() % 4;
        if (dir == 0) direction = { 1.f, 0.f };
        else if (dir == 1) direction = { -1.f, 0.f };
        else if (dir == 2) direction = { 0.f, 1.f };
        else direction = { 0.f, -1.f };
        timeSinceDirectionChange = 0.f;
    }

    // Sau khi trúng đạn → 0.2s sau thì hết hiệu ứng
    if (isHitEffect && hitClock.getElapsedTime().asSeconds() > 0.2f)
    {
        isHitEffect = false;
        body.setFillColor(sf::Color::Red);
    }

    // Bắn đạn thông minh nếu có thể
    smartShoot(playerPos, walls);

    // Cập nhật vị trí đạn
    for (auto& bullet : bullets)
        bullet.update(deltaTime);

    // Xoá đạn khi chạm tường hoặc ra ngoài màn hình
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [&walls, this](const Bullet& bullet) -> bool {
                for (const auto& wall : walls) {
                    if (bullet.getBounds().intersects(wall.getBounds())) {
                        return true;
                    }
                }
                if (windowPtr != nullptr && bullet.isOffScreen(*windowPtr)) {
                    return true;
                }
                return false;
            }),
        bullets.end()
    );
}

// Update không tham số - chỉ gọi lại update chính với dữ liệu rỗng
void Enemy::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    sf::Vector2f dummyPlayerPos(0.0f, 0.0f);
    update(deltaTime, emptyWalls, dummyPlayerPos);
}

// Vẽ enemy, thanh máu, và đạn
void Enemy::draw(sf::RenderWindow& window) const
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

    // Vẽ tất cả đạn
    for (const auto& bullet : bullets)
    {
        bullet.draw(window);
    }
}

// Kiểm tra va chạm đạn → trừ máu
bool Enemy::isHit(const sf::FloatRect& bounds)
{
    if (getBounds().intersects(bounds))
    {
        std::cout << "Enemy hit! HP: " << hp << std::endl;
        takeDamage(1);
        return true;
    }
    return false;
}

// Giảm máu, kích hoạt hiệu ứng trúng đạn, xử lý chết
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

// Bắt đầu hiệu ứng nổ
void Enemy::markToRemove()
{
    if (!explosionTexture.loadFromFile("assets/Images/explosion.png"))
    {
        std::cout << "Không thể tải explosion.png\n";
    }
    else
    {
        explosionSprite.setTexture(explosionTexture);
        explosionSprite.setOrigin(explosionTexture.getSize().x / 2.f, explosionTexture.getSize().y / 2.f);
        explosionSprite.setPosition(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
        isExploding = true;
        explosionTimer = 0.f;
    }
    toBeRemoved = false; // Đợi hiệu ứng xong mới xoá thật
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

// Di chuyển đuổi theo player
void Enemy::chasePlayer(const sf::Vector2f& playerPos, float dt, const std::vector<Wall>& walls)
{
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f dir = playerPos - pos;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0.f)
        dir /= len;

    sf::Vector2f movement = dir * speed * dt;

    // Bounds mới sau khi di chuyển
    sf::FloatRect futureBounds = body.getGlobalBounds();
    futureBounds.left += movement.x;
    futureBounds.top += movement.y;

    bool collision = false;
    for (const Wall& wall : walls)
    {
        if (wall.getBounds().intersects(futureBounds))
        {
            collision = true;
            break;
        }
    }

    // Kiểm tra biên bản đồ
    if (futureBounds.left < 0 || futureBounds.left + futureBounds.width > 800 ||
        futureBounds.top < 0 || futureBounds.top + futureBounds.height > 600)
    {
        collision = true;
    }

    if (!collision)
        body.move(movement);
}

// Getter HP, MaxHP, hồi máu
int Enemy::getMaxHP() const { return maxHp; }

void Enemy::heal(float ratio)
{
    int amount = static_cast<int>(maxHp * ratio);
    hp += amount;
    if (hp > maxHp)
        hp = maxHp;
}

int Enemy::getHP() const { return hp; }

// Kiểm tra đường bắn thẳng (không bị tường chắn) trong phạm vi
bool Enemy::hasLineOfSight(const sf::Vector2f& playerPos, const std::vector<Wall>& walls) const
{
    sf::Vector2f enemyCenter = sf::Vector2f(body.getPosition().x + body.getSize().x / 2,
        body.getPosition().y + body.getSize().y / 2);

    float distance = std::sqrt(std::pow(playerPos.x - enemyCenter.x, 2) +
        std::pow(playerPos.y - enemyCenter.y, 2));
    if (distance > shootRange)
        return false;

    sf::Vector2f direction = playerPos - enemyCenter;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0)
        return false;

    direction /= length; // Chuẩn hoá vector

    // Raycast mỗi 5 pixel
    int steps = static_cast<int>(length / 5.0f);
    for (int i = 1; i < steps; ++i)
    {
        sf::Vector2f checkPoint = enemyCenter + direction * (i * 5.0f);
        sf::FloatRect checkRect(checkPoint.x - 2, checkPoint.y - 2, 4, 4);

        for (const auto& wall : walls)
        {
            if (wall.getBounds().intersects(checkRect))
                return false;
        }
    }

    return true;
}

// Kiểm tra đủ điều kiện bắn (cooldown + tầm nhìn)
bool Enemy::canShoot(const sf::Vector2f& playerPos, const std::vector<Wall>& walls) const
{
    if (shootClock.getElapsedTime().asSeconds() < shootCooldown)
        return false;
    return hasLineOfSight(playerPos, walls);
}

// Bắn đạn thông minh về phía player
void Enemy::smartShoot(const sf::Vector2f& playerPos, const std::vector<Wall>& walls)
{
    if (!canShoot(playerPos, walls))
        return;

    sf::Vector2f enemyCenter = sf::Vector2f(body.getPosition().x + body.getSize().x / 2,
        body.getPosition().y + body.getSize().y / 2);
    sf::Vector2f direction = playerPos - enemyCenter;
    float length = std::sqrt(direction.x * direction.x + direction.y* direction.y);

    if (length != 0.0f)
    {
        direction /= length; // Chuẩn hoá vector
        float bulletSpeed = 250.0f;
        bullets.emplace_back(enemyCenter, direction, bulletSpeed);
        shootClock.restart();

        std::cout << "Enemy bắn đạn về phía player!\n";
    }
}

// Lấy hitbox của enemy (dựa vào sprite)
sf::FloatRect Enemy::getBounds() const {
    return tankSprite.getGlobalBounds();
}
