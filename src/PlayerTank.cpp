// PlayerTank.cpp
#include "PlayerTank.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp> 

PlayerTank::PlayerTank()
{
    speed = 200.f;
    body.setFillColor(sf::Color::Green);
    //thêm máu
    maxHP = 100;
    hp = maxHP;
}

void PlayerTank::handleInput()
{
    movement = { 0.f, 0.f };
    float angle = body.getRotation(); // giữ nguyên nếu không di chuyển

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

    // Nếu di chuyển theo đường chéo
    if (movement.x != 0.f && movement.y != 0.f)
    {
        angle = std::atan2(movement.y, movement.x) * 180.f / 3.14159f;
    }

    body.setRotation(angle);

}
void PlayerTank::draw(sf::RenderWindow &window) const
{
    window.draw(body); // Hoặc bất kỳ logic custom nào bạn muốn
    drawHP(window);
}
void PlayerTank::update(float deltaTime)
{
    handleInput();
    move(movement.x * speed * deltaTime, movement.y * speed * deltaTime);
}
void PlayerTank::move(float dx, float dy)
{
    body.move(dx, dy);
}
void PlayerTank::shoot()
{
}
void PlayerTank::takeDamage(int dmg)
{
    hp -= dmg;
    if (hp < 0)
        hp = 0;
}

int PlayerTank::getHP() const
{
    return hp;
}

void PlayerTank::drawHP(sf::RenderWindow& window) const
{
    sf::RectangleShape back(sf::Vector2f(100, 10));
    back.setFillColor(sf::Color::Red);
    back.setPosition(10, 10); // góc trái màn hình

    sf::RectangleShape front(sf::Vector2f((float)hp / maxHP * 100, 10));
    front.setFillColor(sf::Color::Green);
    front.setPosition(10, 10);

    window.draw(back);
    window.draw(front);
}
