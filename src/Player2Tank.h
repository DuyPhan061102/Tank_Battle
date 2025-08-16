#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Wall.h"
#include <vector>
#include <cmath>
#include <SFML/Audio.hpp> 

class Bullet; 

class Player2Tank : public Tank {
public:
    Player2Tank();
    void handleInput(); 
    void update(float deltaTime) override;
    void move(float dx, float dy) override;
    void draw(sf::RenderWindow& window) const override;

    void shoot();
    void takeDamage(int damage);
    int getHP() const;
    void drawHP(sf::RenderWindow& window) const;
    void updateHealthBar();
    void setWindow(sf::RenderWindow* window);
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets();
    void setShootSound(sf::Sound* sound);
    void setWalls(std::vector<Wall>* walls);
    void reset();
    void resetHP();
    void setTexture(const sf::Texture* texture);
    void healByPercent(float percent);
    void clearBullets();
    void resetExplosion();
    sf::FloatRect getBounds() const;

    
    
private:
    sf::Vector2f movement;
    bool wasEnterPressedLastFrame = false;
    std::vector<Bullet> bullets;
    std::vector<Wall>* wallsPtr = nullptr;
    int currentHealth;
    int maxHealth;
    sf::Texture explosionTexture;
    sf::Sprite explosionSprite;
    bool isExploding = false;
    float explosionTimer = 0.f;
    sf::Sound* shootSoundPtr = nullptr;
    sf::RenderWindow* windowPtr = nullptr;
    sf::Texture tankTexture;
    sf::Sprite tankSprite;
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    
};