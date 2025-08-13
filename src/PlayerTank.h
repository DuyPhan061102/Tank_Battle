// Player.h
#pragma once
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Wall.h"
#include <vector>
#include <cmath>
#include <SFML/Audio.hpp> 

class PlayerTank : public Tank
{
public:
    PlayerTank();


    void handleInput(); // Xử lý input từ bàn phím (WASD, Space)
    void update(float deltaTime) override; // Cập nhật trạng thái (di chuyển, đạn, máu, va chạm)
    void move(float dx, float dy) override; // Di chuyển với kiểm tra va chạm tường
    void draw(sf::RenderWindow &window) const override; // Vẽ Player và HUD máu, đạn


    void shoot(); // Bắn đạn
    void takeDamage(int damage); // Nhận sát thương
    int getHP() const;
    void drawHP(sf::RenderWindow &window) const; // Vẽ thanh máu
    void updateHealthBar(); // Cập nhật thanh máu theo HP hiện tại
    
    void setWindow(sf::RenderWindow* window); // Liên kết cửa sổ SFML
    
    // Lấy danh sách đạn (const hoặc chỉnh sửa)
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets();
    void setShootSound(sf::Sound* sound);  // Gán âm thanh bắn
    void setWalls(std::vector<Wall>* walls); // Gán con trỏ tới danh sách tường (để xử lý va chạm)
    void reset();  // Reset player (HP, vị trí, đạn, góc quay)
    void resetHP();
    void setTexture(const sf::Texture* texture); // Gán texture mới cho PlayerTank
    void healByPercent(float percent); // Hồi máu theo %
    void clearBullets();
    sf::FloatRect getBounds() const;

    

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
    float shootCooldown = 0.3f; // Thời gian giữa 2 lần bắn
    bool wasSpacePressedLastFrame = false; // Kiểm tra giữ phím Space
    
    std::vector<Wall> *wallsPtr = nullptr; // Con trỏ tới danh sách tường
    mutable sf::Clock damageClock; // Đồng hồ chống trúng đạn liên tục
    bool recentlyDamaged = false; // Trạng thái vừa bị bắn

    sf::Vector2f lastPosition;// Vị trí cũ (có thể dùng để hoàn tác di chuyển)


    // hiệu ứng nổ
    sf::Texture explosionTexture;
    sf::Sprite explosionSprite;
    bool isExploding = false; // Trạng thái đang nổ
    float explosionTimer = 0.f; // Thời gian hiệu ứng nổ

};