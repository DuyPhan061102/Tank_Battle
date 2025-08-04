// PlayerTank.cpp
#include "PlayerTank.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>

PlayerTank::PlayerTank()
{
    speed = 200.f;
    body.setFillColor(sf::Color::Green);
    body.setSize(sf::Vector2f(22.f, 22.f)); // hoặc 16.f, 20.f tuỳ ý chỉnh kích thước
    currentHealth = 100;
    maxHealth = 100;

    // Load ảnh tank
    if (!tankTexture.loadFromFile("assets/Images/tank1.png"))
        std::cout << "❌ Không thể tải player_tank.png\n";
    else
    {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setPosition(body.getPosition());
        tankSprite.setScale(0.5f, 0.5f); // thử 0.4 hoặc 0.5 cho nhỏ hơn nữa
    }

    healthBarBack.setSize(sf::Vector2f(100, 10));
    healthBarBack.setFillColor(sf::Color::Red);
    healthBarBack.setPosition(10, 10);

    healthBarFront.setSize(sf::Vector2f(100, 10));
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setPosition(10, 10);

    // Khi player bị trúng đạn hoặc va chạm
    if (!explosionTexture.loadFromFile("assets/Images/explosion2.png"))
        std::cout << "❌ Không thể tải explosion2.png\n";
    // KHÔNG set isExploding, explosionSprite ở đây!
}

void PlayerTank::handleInput()
{
    movement = {0.f, 0.f};
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (!wasSpacePressedLastFrame)
        {
            shoot();
        }
        wasSpacePressedLastFrame = true;
    }
    else
    {
        wasSpacePressedLastFrame = false;
    }
}

void PlayerTank::draw(sf::RenderWindow &window) const
{
    if (isExploding)
        window.draw(explosionSprite);
    else if (tankTexture.getSize().x > 0) {
        sf::Sprite sprite = tankSprite;
        sprite.setPosition(body.getPosition().x + 20, body.getPosition().y + 20);
        sprite.setRotation(body.getRotation());
        window.draw(sprite);
    } else {
        window.draw(body);
    }
    drawHP(window);

    for (const auto &b : bullets)
        b.draw(window);
}
void PlayerTank::update(float deltaTime)
{
    handleInput();
    move(movement.x * speed * deltaTime, movement.y * speed * deltaTime);
    updateHealthBar();

    for (auto &b : bullets)
    {
        b.update(deltaTime);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet &b)
                                 {
                                     sf::Vector2f pos = b.getPosition();
                                     return pos.x < 0 || pos.x > 800 || pos.y < 0 || pos.y > 600;
                                 }),
                  bullets.end());

    if (isExploding)
    {
        explosionTimer += deltaTime;
        if (explosionTimer > 0.4f) // hiệu ứng nổ 0.4 giây
            isExploding = false;
        return;
    }
}
void PlayerTank::move(float dx, float dy)
{
    body.move(dx, dy);
}

sf::Sound *shootSoundPtr = nullptr;

void PlayerTank::setShootSound(sf::Sound *sound)
{
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

    if (shootSoundPtr)
        shootSoundPtr->play();
}
void PlayerTank::takeDamage(int damage)
{
    currentHealth -= damage;
    if (currentHealth < 0)
        currentHealth = 0;

    // Kích hoạt hiệu ứng nổ mỗi lần mất máu
    if (explosionTexture.getSize().x > 0) {
        explosionSprite.setTexture(explosionTexture);
        explosionSprite.setOrigin(explosionTexture.getSize().x / 2.f, explosionTexture.getSize().y / 2.f);
        explosionSprite.setPosition(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
        isExploding = true;
        explosionTimer = 0.f;
    }
}

int PlayerTank::getHP() const
{
    return currentHealth;
}

void PlayerTank::drawHP(sf::RenderWindow &window) const
{
    window.draw(healthBarBack);
    window.draw(healthBarFront);
}

void PlayerTank::updateHealthBar()
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBarFront.setSize(sf::Vector2f(100 * healthPercent, 10));
    sf::Vector2f tankPos = body.getPosition();
    sf::Vector2f hpPos(tankPos.x - 25, tankPos.y - 20); // giảm
    healthBarBack.setPosition(hpPos);
    healthBarFront.setPosition(hpPos);
}

void PlayerTank::setWindow(sf::RenderWindow *window)
{
    windowPtr = window;
}

const std::vector<Bullet> &PlayerTank::getBullets() const
{
    return bullets;
}

std::vector<Bullet> &PlayerTank::getBullets()
{
    return bullets;
}


void PlayerTank::reset() {
    currentHealth = maxHealth;
    bullets.clear();
    body.setPosition(400.f, 500.f);
    body.setRotation(0.f); 
}

void PlayerTank::resetHP()
{
    currentHealth = maxHealth;
    updateHealthBar();
}

