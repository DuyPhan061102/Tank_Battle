#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Enemy : public Tank
{
public:
    Enemy(float x, float y, int hp = 1);
    void move(float dx, float dy) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow &window) const override;
    
    virtual bool isHit(const sf::FloatRect &bounds);
    void markToRemove();
    bool shouldBeRemoved() const;

    void setSpeed(float newSpeed); 
    void takeDamage(int amount);
    void chasePlayer(const sf::Vector2f& playerPos, float dt);
    

private:
    sf::Vector2f direction;
    float timeSinceDirectionChange;

    bool isHitEffect = false;
    sf::Clock hitClock;
    bool toBeRemoved = false;
    int hp;
    int maxHp;

    
};

#endif
