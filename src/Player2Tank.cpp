#include "Player2Tank.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <vector>


Player2Tank::Player2Tank() {
    speed = 200.f;
    body.setFillColor(sf::Color::Blue);
    body.setSize(sf::Vector2f(22.f, 22.f));
    currentHealth = 200;
    maxHealth = 200;

    if (!tankTexture.loadFromFile("assets/Images/tank2.png")) 
        std::cout << "Cannot load tank2.png\n";
    else {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f);
    }

    
    healthBarBack.setSize(sf::Vector2f(100, 10));
    healthBarBack.setFillColor(sf::Color::Red);
    healthBarFront.setSize(sf::Vector2f(100, 10));
    healthBarFront.setFillColor(sf::Color::Cyan); 

    if (!explosionTexture.loadFromFile("assets/Images/explosion2.png"))
        std::cout << "? Kh�ng th? t?i explosion2.png\n";
}

void Player2Tank::handleInput() {
    movement = { 0.f, 0.f };
    float angle = body.getRotation();

  
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        movement.y -= 1.f;
        angle = 270.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        movement.y += 1.f;
        angle = 90.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        movement.x -= 1.f;
        angle = 180.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        movement.x += 1.f;
        angle = 0.f;
    }
    if (movement.x != 0.f || movement.y != 0.f)
    {
        angle = std::atan2(movement.y, movement.x) * 180.f / 3.14159f;
        body.setRotation(angle);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (!wasEnterPressedLastFrame) {
            shoot();
        }
        wasEnterPressedLastFrame = true;
    }
    else {
        wasEnterPressedLastFrame = false;
    }
}

void Player2Tank::update(float deltaTime) {
    handleInput();
    move(movement.x * speed * deltaTime, movement.y * speed * deltaTime);

    updateHealthBar();

    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }
    // X? l? va ch?m �?n <-> t�?ng
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        bool hitWall = false;

        if (wallsPtr) {
            for (Wall& wall : *wallsPtr) {
                if (wall.getBounds().intersects(it->getBounds())) {
                    wall.takeDamage();  // T�ng hitCount
                    hitWall = true;
                    break;
                }
            }
        }

        if (hitWall) {
            it = bullets.erase(it);  // X�a �?n sau khi b?n t�?ng
        }
        else {
            ++it;
        }
    }

    // X�a �?n n?u ra kh?i m�n h?nh
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) {
            sf::Vector2f pos = b.getPosition();
            return pos.x < 0 || pos.x > 800 || pos.y < 0 || pos.y > 600;
        }), bullets.end());

    // Xo� t�?ng �? b? ph� (hitCount >= 3)
    if (wallsPtr) {
        wallsPtr->erase(
            std::remove_if(wallsPtr->begin(), wallsPtr->end(),
                [](const Wall& wall) { return wall.isDestroyed(); }),
            wallsPtr->end());
    }

    if (isExploding)
    {
        explosionTimer += deltaTime;
        if (explosionTimer > 0.4f) // hi?u ?ng n? 0.4 gi�y
            isExploding = false;
        return;
    }

}

void Player2Tank::draw(sf::RenderWindow& window) const 
{
    if (isExploding)
        window.draw(explosionSprite);
    else if (tankTexture.getSize().x > 0)
    {
        sf::Sprite sprite = tankSprite;
        sprite.setPosition(body.getPosition());
        sprite.setRotation(body.getRotation());
        window.draw(sprite);
    }
    else {
        window.draw(body);
    }
    drawHP(window);

    for (const auto& b : bullets)
        b.draw(window);
}

void Player2Tank::move(float dx, float dy) {
    sf::FloatRect futureBounds = body.getGlobalBounds();
    futureBounds.left += dx;
    futureBounds.top += dy;

    bool collision = false;
    if (wallsPtr) {
        for (const Wall& wall : *wallsPtr) {
            if (wall.getBounds().intersects(futureBounds)) {
                collision = true;
                break;
            }
        }
    }

    if (!collision)
        body.move(dx, dy);
}


void Player2Tank::shoot() {
    float rotation = body.getRotation();
    float rad = rotation * 3.14159265f / 180.f;
    sf::Vector2f direction(std::cos(rad), std::sin(rad));
    sf::Vector2f startPos = body.getPosition() + direction * 30.f;
    float bulletSpeed = 300.f;
    Bullet newBullet(startPos, direction, bulletSpeed);
    bullets.push_back(newBullet);

    if (shootSoundPtr)
        shootSoundPtr->play();
}



void Player2Tank::setShootSound(sf::Sound* sound)
{
    shootSoundPtr = sound;
}

void Player2Tank::drawHP(sf::RenderWindow& window) const
{
    window.draw(healthBarBack);
    window.draw(healthBarFront);
}

void Player2Tank::setWindow(sf::RenderWindow* window)
{
    windowPtr = window;
}

const std::vector<Bullet>& Player2Tank::getBullets() const
{
    return bullets;
}

std::vector<Bullet>& Player2Tank::getBullets()
{
    return bullets;
}

void Player2Tank::setTexture(const sf::Texture* texture)
{
    if (texture) {
        tankTexture = *texture; 
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f);
    }
}

void Player2Tank::takeDamage(int damage)
{
    currentHealth -= damage;
    if (currentHealth < 0)
        currentHealth = 0;

    if (currentHealth == 0 && explosionTexture.getSize().x > 0) {
        explosionSprite.setTexture(explosionTexture);
        explosionSprite.setOrigin(explosionTexture.getSize().x / 2.f, explosionTexture.getSize().y / 2.f);
        explosionSprite.setPosition(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
        isExploding = true;
        explosionTimer = 0.f;
    }
}

void Player2Tank::resetHP() {
    currentHealth = maxHealth;
    updateHealthBar();
}

int Player2Tank::getHP() const
{
    return currentHealth;
}
void Player2Tank::setWalls(std::vector<Wall>* walls) {
    wallsPtr = walls;
}

void Player2Tank::updateHealthBar()
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBarFront.setSize(sf::Vector2f(100 * healthPercent, 10));
    sf::Vector2f tankPos = body.getPosition();
    sf::Vector2f hpPos(tankPos.x - 25, tankPos.y - 20); // gi?m
    healthBarBack.setPosition(hpPos);
    healthBarFront.setPosition(hpPos);
}
void Player2Tank::clearBullets()
{
    bullets.clear();
}
void Player2Tank::resetExplosion()
{
    isExploding = false;
    explosionTimer = 0.f;
}