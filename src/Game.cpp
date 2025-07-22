#include <iostream>
#include "Game.h"
#include "Enemy.h"
#include <cstdlib>

Game::Game() : window(sf::VideoMode(800, 600), "Tank Battle"), isRunning(true)
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(time(nullptr)));
    score = 0;

    // tai background
    if (!backgroundTexture.loadFromFile("assets/Images/background.jpg"))
    {
        std::cout << "❌ Không thể tải background.jpg\n";
    }
    else
    {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            window.getSize().x / backgroundSprite.getLocalBounds().width,
            window.getSize().y / backgroundSprite.getLocalBounds().height);
    }

    // tai font
    if (!font.loadFromFile("assets/Fonts/arial.ttf"))
    {
        std::cout << "❌ Không thể tải font arial.ttf\n";
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setString("Score: 0");
    waveText.setFont(font);
    waveText.setCharacterSize(24);
    waveText.setFillColor(sf::Color::Cyan);
    waveText.setPosition(10.f, 40.f);
    waveText.setString("Wave: 1");


    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(250.f, 250.f);

    // Tải âm thanh bắn
    if (!shootBuffer.loadFromFile("assets/Sounds/shoot.wav"))
    {
        std::cout << "❌ Không thể tải file shoot.wav\n";
    }
    else
    {
        shootSound.setBuffer(shootBuffer);
        shootSound.setVolume(100.f);
    }

    // Tải âm thanh nổ
    if (!explosionBuffer.loadFromFile("assets/Sounds/explosion.wav"))
    {
        std::cout << "❌ Không thể tải file explosion.wav\n";
    }
    else
    {
        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(100.f);
    }
}

void Game::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(float dt)
{
    if (!isRunning)
        return;

    player.update(dt);

    // -- BẮN ĐẠN nếu nhấn phím SPACE --

    static sf::Clock shootClock;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (shootClock.getElapsedTime().asMilliseconds() > 300)
        {
            sf::Vector2f startPos = player.getPosition();
            sf::Vector2f dir(1.f, 0.f);
            bullets.emplace_back(startPos, dir);
            shootSound.play();
            shootClock.restart();
        }
    }

    // Sinh enemy mỗi 3s
    if (enemySpawnClock.getElapsedTime().asSeconds() > 3.f)
    {
        spawnEnemy();
        enemySpawnClock.restart();
    }

    // Cập nhật vị trí enemy

    for (auto &enemy : enemies)
    {
        enemy.update(dt);
    }

    // -- Cập nhật vị trí đạn --
    for (auto &bullet : bullets)
    {
        bullet.update(dt);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [this](const Bullet &b)
                                 {
                                     return b.isOffScreen(window);
                                 }),
                  bullets.end());

    // -- Va chạm giữa đạn và enemy --
    // THAY ĐỔI: không xóa enemy tại đây, để nó có thời gian hiện hiệu ứng

    for (auto b = bullets.begin(); b != bullets.end();)
    {
        bool bulletErased = false;
        for (auto &enemy : enemies)
        {
            if (enemy.isHit(b->getBounds()))
            {
                b = bullets.erase(b); // Xoá đạn
                explosionSound.play();

                enemy.markToRemove(); // cần thêm phương thức này trong Enemy
                bulletErased = true;

                score += 100;
                scoreText.setString("Score: " + std::to_string(score));
                break;
            }
        }
        if (!bulletErased)
            ++b;
    }

    // THAY ĐỔI: xoá enemy đã chết sau hiệu ứng
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const Enemy &e)
                       { return e.shouldBeRemoved(); }),
        enemies.end());
    // va ham voii nguoi choi
    if (enemies.empty() && enemySpawnedCount >= enemyPerWave)
 {
    waveNumber++;
    enemyPerWave += 2;
    enemySpawnedCount = 0;
    waveText.setString("Wave: " + std::to_string(waveNumber));

    player.setPosition(sf::Vector2f(100.f, 100.f)); // Reset vị trí player
 }

    sf::FloatRect playerBounds(player.getPosition().x, player.getPosition().y, 40.f, 40.f);
    for (auto &enemy : enemies)
    {
        if (enemy.isHit(playerBounds))
        {
            isRunning = false;
            break;
        }
    }
}

void Game::render()
{
    window.clear();
    window.draw(backgroundSprite);
    // Vẽ player
    player.draw(window);

    for (const auto &bullet : bullets)
    {
        bullet.draw(window);
    }

    // Vẽ enemy

    for (const auto &enemy : enemies)
    {
        enemy.draw(window);
    }

    window.draw(scoreText);
    window.draw(waveText);


    if (!isRunning)
    {
        window.draw(gameOverText);
    }

    window.display();
}

void Game::spawnEnemy()
{
    if (enemySpawnedCount >= enemyPerWave)
        return;

    float x = static_cast<float>(rand() % 700 + 50);
    float y = static_cast<float>(rand() % 500 + 50);

    Enemy e(x, y);
    e.setSpeed(50.f * std::pow(1.35f, waveNumber)); // tăng 35% mỗi wave


    enemies.push_back(e);
    enemySpawnedCount++;
}

