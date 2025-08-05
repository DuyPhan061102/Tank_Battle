// Tank.h
#pragma once
#include <SFML/Graphics.hpp>

class Tank
{
protected:
    sf::RectangleShape body;
    float speed;

public:
    Tank();
    virtual ~Tank() = default;
    virtual void update(float deltaTime) = 0;
    virtual void move(float dx, float dy) = 0;
    virtual void draw(sf::RenderWindow &window) const;
    void setPosition(const sf::Vector2f &pos); 
    sf::Vector2f getPosition() const;
    float getSpeed() const;
};