// PlayerTank.cpp
#include "PlayerTank.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include <cmath>

PlayerTank::PlayerTank()
{
    speed = 200.f;
    body.setFillColor(sf::Color::Green);
    currentHealth = 100;
    maxHealth = 100;

    healthBarBack.setSize(sf::Vector2f(100, 10));
    healthBarBack.setFillColor(sf::Color::Red);
    healthBarBack.setPosition(10, 10);

    healthBarFront.setSize(sf::Vector2f(100, 10));
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setPosition(10, 10);
}

void PlayerTank::handleInput()
{
    movement = { 0.f, 0.f };
    float angle = body.getRotation();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement.y -= 1.f;
        angle = 270.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += 1.f;
        angle = 90.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= 1.f;
        angle = 180.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += 1.f;
        angle = 0.f;
    }

    if (movement.x != 0.f || movement.y != 0.f)
    {
        angle = std::atan2(movement.y, movement.x) * 180.f / 3.14159f;
        body.setRotation(angle);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (!wasSpacePressedLastFrame) {
            shoot();  
        }
        wasSpacePressedLastFrame = true;
    }
    else {
        wasSpacePressedLastFrame = false;  
    }
}

void PlayerTank::draw(sf::RenderWindow& window) const
{
    window.draw(body);
    drawHP(window);

    for (const auto& b : bullets)
        b.draw(window);
}

//update move
void PlayerTank::move(float dx, float dy)
{
    sf::FloatRect bodyBounds = body.getGlobalBounds();
    sf::FloatRect futureBounds = bodyBounds;
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
void PlayerTank::update(float deltaTime)
{
    handleInput();

    // Di chuyển theo input
    move(movement.x * speed * deltaTime, movement.y * speed * deltaTime);

    // Cập nhật thanh máu
    updateHealthBar();

    // Cập nhật vị trí đạn
    for (auto& b : bullets)
        b.update(deltaTime);

    // Xử lý va chạm đạn <-> tường
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        bool hitWall = false;

        if (wallsPtr) {
            for (Wall& wall : *wallsPtr) {
                if (wall.getBounds().intersects(it->getBounds())) {
                    wall.takeDamage();  // Tăng hitCount
                    hitWall = true;
                    break;
                }
            }
        }

        if (hitWall) {
            it = bullets.erase(it);  // Xóa đạn sau khi bắn tường
        } else {
            ++it;
        }
    }

    // Xóa đạn nếu ra khỏi màn hình
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) {
            sf::Vector2f pos = b.getPosition();
            return pos.x < 0 || pos.x > 800 || pos.y < 0 || pos.y > 600;
        }), bullets.end());

    // Xoá tường đã bị phá (hitCount >= 3)
    if (wallsPtr) {
        wallsPtr->erase(
            std::remove_if(wallsPtr->begin(), wallsPtr->end(),
                           [](const Wall& wall) { return wall.isDestroyed(); }),
            wallsPtr->end());
    }
}




sf::Sound* shootSoundPtr = nullptr;

void PlayerTank::setShootSound(sf::Sound* sound) {
    shootSoundPtr = sound;
}

void PlayerTank::shoot()
{
    float rotation = body.getRotation(); 
    float rad = rotation * 3.14159265f / 180.f; 

    sf::Vector2f direction(std::cos(rad), std::sin(rad)); 

    sf::Vector2f startPos = body.getPosition() + direction * 30.f; 

    float bulletSpeed = 300.f;
    Bullet newBullet(startPos, direction, bulletSpeed);
    bullets.push_back(newBullet);

    if (shootSoundPtr) shootSoundPtr->play();
}
void PlayerTank::takeDamage(int damage)
{
    currentHealth -= damage;
    if (currentHealth < 0)
        currentHealth = 0;
}

int PlayerTank::getHP() const
{
    return currentHealth;
}

void PlayerTank::drawHP(sf::RenderWindow& window) const
{
    window.draw(healthBarBack);
    window.draw(healthBarFront);
}

void PlayerTank::updateHealthBar()
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBarFront.setSize(sf::Vector2f(100 * healthPercent, 10));
    sf::Vector2f tankPos = body.getPosition();
    sf::Vector2f hpPos(tankPos.x - 50, tankPos.y - 40);
    healthBarBack.setPosition(hpPos);
    healthBarFront.setPosition(hpPos);
}

void PlayerTank::setWindow(sf::RenderWindow* window)
{
    windowPtr = window;
}

const std::vector<Bullet>& PlayerTank::getBullets() const {
    return bullets;
}

std::vector<Bullet>& PlayerTank::getBullets() {
    return bullets;
}
void PlayerTank::setWalls(std::vector<Wall>* walls) {
    wallsPtr = walls;
}
