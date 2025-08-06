// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include"Wall.h"

class Enemy : public Tank
{
public:
 Enemy(float x, float y, int hp = 1);

    void move(float dx, float dy) override;
    void update(float deltaTime) override; // override hàm thuần ảo của Tank
    virtual void update(float deltaTime, const std::vector<Wall>& walls); // hàm riêng cho Enemy
    void draw(sf::RenderWindow &window) const override;

    void setSpeed(float newSpeed);
    virtual bool isHit(const sf::FloatRect &bounds); // virtual để lớp con override nếu cần
    void markToRemove();
    bool shouldBeRemoved() const;

    void takeDamage(int amount);
    void chasePlayer(const sf::Vector2f& playerPos, float dt, const std::vector<Wall>& walls);
    virtual bool isBoss() const { return false; }
    int getMaxHP() const;
    void heal(float ratio);
protected:
    sf::Texture tankTexture;
    sf::Sprite tankSprite;
    sf::Vector2f direction;
    float timeSinceDirectionChange;
    bool isHitEffect = false;
    bool toBeRemoved = false;
    sf::Clock hitClock;

    // hiệu ứng nổ
    sf::Texture explosionTexture;
    sf::Sprite explosionSprite;
    bool isExploding = false;
    float explosionTimer = 0.f;

    int hp;
    int maxHp;
};

#endif