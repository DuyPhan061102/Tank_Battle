// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Wall.h"
#include "Bullet.h"

class Enemy : public Tank
{
public:
    Enemy(float x, float y, int health);

    void move(float dx, float dy) override;
    void update(float deltaTime) override; // override hàm thuần ảo của Tank
    virtual void update(float deltaTime, const std::vector<Wall> &walls, const sf::Vector2f &playerPos);
    void draw(sf::RenderWindow &window) const override;

    void setSpeed(float newSpeed);
    virtual bool isHit(const sf::FloatRect &bounds); // virtual để lớp con override nếu cần
    void markToRemove();
    bool shouldBeRemoved() const;

    void takeDamage(int amount);
    void chasePlayer(const sf::Vector2f &playerPos, float dt, const std::vector<Wall> &walls);
    virtual bool isBoss() const { return false; }
    int getMaxHP() const;
    void heal(float ratio);
    int getHP() const;
    
    // Hàm bắn đạn thông minh
    bool hasLineOfSight(const sf::Vector2f &playerPos, const std::vector<Wall> &walls) const;
    bool canShoot(const sf::Vector2f &playerPos, const std::vector<Wall> &walls) const;
    void smartShoot(const sf::Vector2f &playerPos, const std::vector<Wall> &walls);
    
    // Quản lý đạn
    std::vector<Bullet>& getBullets() { return bullets; }
    const std::vector<Bullet>& getBullets() const { return bullets; }
    void setWindow(sf::RenderWindow* window) { windowPtr = window; }

protected:
    sf::Texture tankTexture;
    sf::Sprite tankSprite;
    sf::Vector2f direction;
    float timeSinceDirectionChange;
    bool isHitEffect = false;
    bool toBeRemoved = false;
    sf::Clock hitClock;

    // Hiệu ứng nổ
    sf::Texture explosionTexture;
    sf::Sprite explosionSprite;
    bool isExploding = false;
    float explosionTimer = 0.f;

    int hp;
    int maxHp;
    bool isDead = false;
    
    // Hệ thống bắn đạn
    std::vector<Bullet> bullets;
    sf::Clock shootClock;
    float shootCooldown = 2.0f;
    float shootRange = 300.0f;

private:
    sf::RenderWindow* windowPtr = nullptr;
};

#endif