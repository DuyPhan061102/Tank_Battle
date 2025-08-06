#pragma once
#include "Enemy.h"

class EnemyScout : public Enemy
{
public:
    EnemyScout(float x, float y);
    void update(float deltaTime) override;
    void update(float deltaTime, const std::vector<Wall> &walls) override;
};
