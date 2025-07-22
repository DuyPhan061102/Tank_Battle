// PlayerTank.h
#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include <cmath>


class PlayerTank : public Tank
{
public:
    PlayerTank();

    void handleInput();
    void update(float deltaTime) override;
    void move(float dx, float dy) override;
    void draw(sf::RenderWindow &window) const override;

    void shoot();

    void takeDamage(int dmg);
    int getHP() const;
    void drawHP(sf::RenderWindow& window) const;

private:
    sf::Vector2f movement;
    int hp;
    int maxHP;
};
