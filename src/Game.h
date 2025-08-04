//Game.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "PlayerTank.h"
#include "Tank.h"
#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <cmath>
#include <fstream> // để đọc/ghi file
#include <memory>

// Trạng thái của game
enum class GameState
{
    Menu,
    Playing,
    GameOver,
    MoreMenu,
    Tutorial,
    Volume,
};

class Game
{
private:
    int waveNumber = 1;
    int enemyPerWave = 3;
    int enemySpawnedCount = 0;
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Clock enemySpawnClock;

    GameState gameState;     // trạng thái hiện tại của game
    bool isRunning;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Bullet> bullets;
    PlayerTank player;

    // ảnh nền
    sf::Texture backgroundTexture; // ảnh nền
    sf::Sprite backgroundSprite;   // sprite để vẽ ảnh

    // ảnh menu
    sf::Texture menuBackgroundTexture; // ảnh nền menu
    sf::Sprite menuBackgroundSprite;

    // Font và văn bản
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text waveText;
    sf::Text highScoreText;
    sf::Text highScoreMenuText;
    sf::Text gameTitle;

    // Các nút menu và game over
    sf::Text playButton;
    sf::Text quitButton;
    sf::Text retryButton;
    sf::Text exitButton;
    sf::Text moreButton;
    sf::Text tutorialButton;
    sf::Text volumeButton;
    sf::Text highScoreButton;
    sf::Text backButton;
    sf::Text returnButton;

    // Ô cho các nút menu
    sf::RectangleShape playButtonBox;
    sf::RectangleShape quitButtonBox;
    sf::RectangleShape retryButtonBox;
    sf::RectangleShape exitButtonBox;
    sf::RectangleShape moreButtonBox;
    sf::RectangleShape tutorialButtonBox;
    sf::RectangleShape volumeButtonBox;
    sf::RectangleShape highScoreButtonBox;
    sf::RectangleShape backButtonBox;
    sf::RectangleShape returnButtonBox;

    // Menu tutorial
    sf::RectangleShape tutorialBox;
    sf::Text tutorialText;
    sf::Sprite wasdSprite, spaceSprite, escSprite;
    sf::Texture wasdTexture, spaceTexture, escTexture;

    // Volume menu
    sf::RectangleShape musicButton;
    sf::Text musicText;
    sf::RectangleShape sfxButton;
    sf::Text sfxText;

    // Âm thanh cho nút
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;

    // Âm thanh
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::Sound shootSound;
    sf::Sound explosionSound;

    // Bật tắt nhạc
    bool isMusicOn = true;
    bool isSFXOn = true;

    // Điểm số
    int score = 0;
    int highScore = 0;
    bool showHighScoreText = false;

    sf::Music backgroundMusic; // cài nhạc nền

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

    void loadAssets();
    void setupBackgrounds();
    void setupFontsAndText();
    void setupTutorial();
    void setupUIButtons();
    void setupAudio();
    void setupButtonText(sf::Text& text, const std::string& str, unsigned int charSize);
    void updateMenuButtonHovers();
};
