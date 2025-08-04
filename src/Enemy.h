// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Enemy : public Tank
{
public:
    Enemy(float x, float y);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow &window) const override;
    void move(float dx, float dy) override;
    void setSpeed(float newSpeed);
    bool isHit(const sf::FloatRect &bounds);
    void markToRemove();
    bool shouldBeRemoved() const;

private:
    sf::Texture tankTexture;
    sf::Sprite tankSprite;
    sf::Vector2f direction;
    float timeSinceDirectionChange;
    bool isHitEffect;
    bool toBeRemoved;
    sf::Clock hitClock;
    //hiệu ứng nổ
    sf::Texture explosionTexture;
    sf::Sprite explosionSprite;
    bool isExploding = false;
    float explosionTimer = 0.f;
};

#endif
