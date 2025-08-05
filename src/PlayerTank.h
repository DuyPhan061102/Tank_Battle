// Player.h
#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Wall.h"
#include <vector>
#include <cmath>
#include <SFML/Audio.hpp> // Thêm dòng này

class PlayerTank : public Tank
{
public:
    PlayerTank();

    void handleInput();
    void update(float deltaTime) override;
    void move(float dx, float dy) override;
    void draw(sf::RenderWindow &window) const override;

    sf::Clock& getWallDamageClock();
    float getWallDamageCooldown() const;


    void shoot();

    void takeDamage(int dmg);
    int getHP() const;
    void drawHP(sf::RenderWindow &window) const;
    void updateHealthBar();
    
    void setWindow(sf::RenderWindow *window);
    const std::vector<Bullet> &getBullets() const;
    std::vector<Bullet> &getBullets();
    void setShootSound(sf::Sound *sound);
    void setWalls(std::vector<Wall> *walls);
    void reset();
    void resetHP();
    void setTexture(const sf::Texture* texture);

private:
    sf::Texture tankTexture;
    sf::Sprite tankSprite;
    sf::Vector2f movement;
    int maxHealth;
    int currentHealth;
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    std::vector<Bullet> bullets;
    sf::RenderWindow *windowPtr = nullptr;
    sf::Clock shootClock;
    float shootCooldown = 0.3f;
    bool wasSpacePressedLastFrame = false;
    
    std::vector<Wall> *wallsPtr = nullptr;
    mutable sf::Clock damageClock;
    bool recentlyDamaged = false;
    sf::Vector2f lastPosition;

    // hiệu ứng nổ
    sf::Texture explosionTexture;
    sf::Sprite explosionSprite;
    bool isExploding = false;
    float explosionTimer = 0.f;
};