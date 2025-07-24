// PlayerTank.h
#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include <vector>


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
    void updateHealthBar();
    void setWindow(sf::RenderWindow* window);
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets();
    


private:
    sf::Vector2f movement;
    int maxHealth;
    int currentHealth;
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    std::vector<Bullet> bullets;
    sf::RenderWindow* windowPtr = nullptr;
    sf::Clock shootClock;
    float shootCooldown = 0.3f;
};
