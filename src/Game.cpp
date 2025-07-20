#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "Enemy.h"

Game::Game() : window(sf::VideoMode(800, 600), "Tank Battle"), isRunning(true), gameState(GameState::Menu)
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(time(nullptr)));
    score = 0;

    if (!backgroundTexture.loadFromFile("assets/Images/background.jpg"))
        std::cout << "❌ Không thể tải background.jpg\n";
    else
    {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            window.getSize().x / backgroundSprite.getLocalBounds().width,
            window.getSize().y / backgroundSprite.getLocalBounds().height);
    }

    if (!font.loadFromFile("assets/Fonts/arial.ttf"))
        std::cout << "❌ Không thể tải font arial.ttf\n";

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setString("Score: 0");

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(250.f, 150.f);

    // Buttons
    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(36);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(300.f, 250.f);

    quitButton.setFont(font);
    quitButton.setString("Quit");
    quitButton.setCharacterSize(36);
    quitButton.setFillColor(sf::Color::White);
    quitButton.setPosition(300.f, 320.f);

    retryButton.setFont(font);
    retryButton.setString("Retry");
    retryButton.setCharacterSize(36);
    retryButton.setFillColor(sf::Color::White);
    retryButton.setPosition(300.f, 250.f);

    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(36);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setPosition(300.f, 320.f);

    if (!shootBuffer.loadFromFile("assets/Sounds/shoot.wav"))
        std::cout << "❌ Không thể tải file shoot.wav\n";
    else
    {
        shootSound.setBuffer(shootBuffer);
        shootSound.setVolume(100.f);
    }

    if (!explosionBuffer.loadFromFile("assets/Sounds/explosion.wav"))
        std::cout << "❌ Không thể tải file explosion.wav\n";
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

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (gameState == GameState::Menu)
            {
                if (playButton.getGlobalBounds().contains(mousePos))
                {
                    gameState = GameState::Playing;
                    isRunning = true;
                    score = 0;
                    scoreText.setString("Score: 0");
                    bullets.clear();
                    enemies.clear();
                }
                else if (quitButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }
            }
            else if (gameState == GameState::GameOver)
            {
                if (retryButton.getGlobalBounds().contains(mousePos))
                {
                    gameState = GameState::Playing;
                    isRunning = true;
                    score = 0;
                    scoreText.setString("Score: 0");
                    bullets.clear();
                    enemies.clear();
                }
                else if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }
            }
        }
    }
}

void Game::update(float dt)
{
    if (gameState == GameState::Menu || gameState == GameState::GameOver)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (gameState == GameState::Menu)
        {
            playButton.setFillColor(playButton.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
            quitButton.setFillColor(quitButton.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
        }
        else if (gameState == GameState::GameOver)
        {
            retryButton.setFillColor(retryButton.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
            exitButton.setFillColor(exitButton.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
        }

        return;
    }

    if (!isRunning)
        return;

    player.update(dt);

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

    if (enemySpawnClock.getElapsedTime().asSeconds() > 3.f)
    {
        spawnEnemy();
        enemySpawnClock.restart();
    }

    for (auto& enemy : enemies)
        enemy.update(dt);

    for (auto& bullet : bullets)
        bullet.update(dt);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [this](const Bullet& b)
        {
            return b.isOffScreen(window);
        }),
        bullets.end());

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

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e)
            { return e.shouldBeRemoved(); }),
        enemies.end());

    sf::FloatRect playerBounds(player.getPosition().x, player.getPosition().y, 40.f, 40.f);
    for (auto& enemy : enemies)
    {
        if (enemy.isHit(playerBounds))
        {
            isRunning = false;
            gameState = GameState::GameOver;
            break;
        }
    }
}

void Game::render()
{
    window.clear();
    window.draw(backgroundSprite);

    if (gameState == GameState::Menu)
    {
        window.draw(playButton);
        window.draw(quitButton);
    }
    else if (gameState == GameState::Playing)
    {
        player.draw(window);

        for (const auto& bullet : bullets)
            bullet.draw(window);

        for (const auto& enemy : enemies)
            enemy.draw(window);

        window.draw(scoreText);

        if (!isRunning)
        {
            window.draw(gameOverText);
        }
    }
    else if (gameState == GameState::GameOver)
    {
        window.draw(gameOverText);
        window.draw(scoreText);
        window.draw(retryButton);
        window.draw(exitButton);
    }

    window.display();
}

void Game::spawnEnemy()
{
    float x = static_cast<float>(rand() % 700 + 50);
    float y = static_cast<float>(rand() % 500 + 50);
    enemies.emplace_back(x, y);
}
