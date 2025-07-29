#pragma once
#include "Enemy.h"

class EnemyTank : public Enemy {
public:
    EnemyTank(float x, float y);
    void update(float deltaTime) override;
};
