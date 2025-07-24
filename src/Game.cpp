#include <iostream>
#include "Game.h"
#include "Enemy.h"
#include <cstdlib>

Game::Game() : window(sf::VideoMode(800, 600), "Tank Battle"), isRunning(true)
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(time(nullptr)));
    score = 0;
    player.setWindow(&window);

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
    if (isRunning)
    {
        player.handleInput(); 
    }
}

void Game::update(float dt)
{
    if (!isRunning)
        return;

    player.update(dt);

    // Spawn enemy mỗi 3 giây
    if (enemySpawnClock.getElapsedTime().asSeconds() > 3.f)
    {
        spawnEnemy();
        enemySpawnClock.restart();
    }

    for (auto& enemy : enemies)
        enemy.update(dt);

    // Cập nhật đạn
    auto& bullets = player.getBullets();
    for (auto& bullet : bullets)
        bullet.update(dt);

    // Xử lý va chạm đạn <-> enemy
    for (auto b = bullets.begin(); b != bullets.end();)
    {
        bool bulletErased = false;
        for (auto& enemy : enemies)
        {
            if (enemy.isHit(b->getBounds()))
            {
                b = bullets.erase(b);
                explosionSound.play();
                enemy.markToRemove();
                bulletErased = true;

                score += 100;
                scoreText.setString("Score: " + std::to_string(score));
                break;
            }
        }
        if (!bulletErased)
            ++b;
    }

    // Xóa enemy đã chết
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return e.shouldBeRemoved(); }),
        enemies.end());

    // Va chạm enemy với player
    sf::FloatRect playerBounds(player.getPosition().x, player.getPosition().y, 40.f, 40.f);
    for (auto& enemy : enemies)
    {
        if (enemy.isHit(playerBounds))
        {
            player.takeDamage(20);

            if (player.getHP() <= 0)
            {
                isRunning = false;
            }
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

    

    // Vẽ enemy

    for (const auto &enemy : enemies)
    {
        enemy.draw(window);
    }

    window.draw(scoreText);

    if (!isRunning)
    {
        window.draw(gameOverText);
    }

    window.display();
}

void Game::spawnEnemy()
{
    float x = static_cast<float>(rand() % 700 + 50);
    float y = static_cast<float>(rand() % 500 + 50);
    enemies.emplace_back(x, y);
}
