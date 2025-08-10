#pragma once
#include "Enemy.h"

class EnemyBoss : public Enemy
{
private:
    int hp;

public:
    EnemyBoss(float x, float y);
    void update(float deltaTime) override;
void update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos) override;    bool isHit(const sf::FloatRect &bounds) override;
    bool isBoss() const override { return true; }
};
