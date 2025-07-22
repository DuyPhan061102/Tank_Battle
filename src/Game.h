#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "PlayerTank.h"
#include "Tank.h"
#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <fstream> // để đọc/ghi file


// Trạng thái của game
enum class GameState
{
    Menu,
    Playing,
    GameOver
};

class Game
{
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Clock enemySpawnClock;

    GameState gameState;     // trạng thái hiện tại của game
    bool isRunning;

    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    PlayerTank player;

    // ảnh nền
    sf::Texture backgroundTexture; // ảnh nền
    sf::Sprite backgroundSprite;   // sprite để vẽ ảnh

    // Font và văn bản
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;

    // Các nút menu và game over
    sf::Text playButton;
    sf::Text quitButton;
    sf::Text retryButton;
    sf::Text exitButton;

    // Điểm số
    int score = 0;

    // Âm thanh
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::Sound shootSound;
    sf::Sound explosionSound;

    // Điểm cao nhất
    int highScore = 0;
    sf::Text highScoreText;

    void loadHighScore(); // đọc từ file
    void saveHighScore(); // ghi vào file

public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void spawnEnemy();
};
