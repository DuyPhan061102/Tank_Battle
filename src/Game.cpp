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
    loadHighScore();
    player.setWindow(&window);
    player.setShootSound(&shootSound);

    if (!backgroundTexture.loadFromFile("assets/Images/background.jpg"))
        std::cout << "❌ Không thể tải background.jpg\n";
    else {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            window.getSize().x / backgroundSprite.getLocalBounds().width,
            window.getSize().y / backgroundSprite.getLocalBounds().height);
    }

    if (!menuBackgroundTexture.loadFromFile("assets/Images/menu_background.jpg"))
        std::cout << "❌ Không thể tải menu_background.jpg\n";
    else {
        menuBackgroundSprite.setTexture(menuBackgroundTexture);
        menuBackgroundSprite.setScale(
            window.getSize().x / menuBackgroundSprite.getLocalBounds().width,
            window.getSize().y / menuBackgroundSprite.getLocalBounds().height);
    }

    if (!font.loadFromFile("assets/Fonts/arial.ttf"))
        std::cout << "❌ Không thể tải font arial.ttf\n";

    // Load key textures
    if (!wasdTexture.loadFromFile("assets/Images/key_wasd.png"))
        std::cout << "Không thể tải ảnh WASD\n";
    wasdSprite.setTexture(wasdTexture);
    wasdSprite.setPosition(60.f, 300.f);  // Tuỳ chỉnh vị trí cho đẹp
    wasdSprite.setScale(0.4f, 0.4f);       // Nếu ảnh to quá

    if (!spaceTexture.loadFromFile("assets/Images/key_space.png"))
        std::cout << "Không thể tải ảnh SPACE\n";
    spaceSprite.setTexture(spaceTexture);
    spaceSprite.setPosition(250.f, 320.f);
    spaceSprite.setScale(0.3f, 0.3f);

    if (!escTexture.loadFromFile("assets/Images/key_esc.png"))
        std::cout << "Không thể tải ảnh ESC\n";
    escSprite.setTexture(escTexture);
    escSprite.setPosition(550.f, 320.f);
    escSprite.setScale(0.5f, 0.5f);


    // Tutorial box
    tutorialBox.setSize(sf::Vector2f(700.f, 450.f));
    tutorialBox.setFillColor(sf::Color(100, 100, 100, 200));
    tutorialBox.setOutlineThickness(3.f);
    tutorialBox.setOutlineColor(sf::Color::Black);
    tutorialBox.setPosition(50.f, 75.f);

    // Tutorial text
    tutorialText.setFont(font);
    tutorialText.setCharacterSize(22);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setString("Instructions:\n\n- Use W / A / S / D to move the tank.\n- Press SPACE to shoot.\n- Press ESC to return in Menu.");
    tutorialText.setPosition(80.f, 100.f);

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
    gameOverText.setPosition(250.f, 150.f);

    float centerX = window.getSize().x / 2.f;

    auto setupButton = [&](sf::Text& text, const std::string& str, float y) {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);
        text.setScale(1.3f, 1.3f);
        float width = text.getGlobalBounds().width * 1.3f;
        float height = text.getGlobalBounds().height * 1.3f;
        text.setPosition(centerX - width / 2.f, y);
        };

    setupButton(playButton, "Play", 250.f);
    setupButton(quitButton, "Quit", 330.f);
    setupButton(moreButton, "More", 410.f);
    setupButton(retryButton, "Retry", 250.f);
    setupButton(exitButton, "Exit", 330.f);
    setupButton(tutorialButton, "Tutorial", 200.f);
    setupButton(volumeButton, "Volume", 280.f);
    setupButton(highScoreButton, "Highest Score", 360.f);

    auto setupBox = [](sf::RectangleShape& box, const sf::Text& text) {
        box.setSize(sf::Vector2f(text.getGlobalBounds().width + 40.f, text.getGlobalBounds().height + 30.f));
        box.setFillColor(sf::Color(150, 150, 150, 180));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::Black);
        box.setPosition(text.getPosition().x - 20.f, text.getPosition().y - 15.f);
        };

    setupBox(playButtonBox, playButton);
    setupBox(quitButtonBox, quitButton);
    setupBox(moreButtonBox, moreButton);
    setupBox(retryButtonBox, retryButton);
    setupBox(exitButtonBox, exitButton);
    setupBox(tutorialButtonBox, tutorialButton);
    setupBox(volumeButtonBox, volumeButton);
    setupBox(highScoreButtonBox, highScoreButton);
    setupBox(musicButton, musicText);
    setupBox(sfxButton, sfxText);

    // Volume Toggle Text
    musicText.setFont(font);
    musicText.setCharacterSize(28);
    musicText.setFillColor(sf::Color::White);
    musicText.setString("MUSIC: ON");
    musicText.setScale(1.1f, 1.1f);
    musicText.setPosition(centerX - musicText.getGlobalBounds().width / 2.f, 200.f);

    sfxText.setFont(font);
    sfxText.setCharacterSize(28);
    sfxText.setFillColor(sf::Color::White);
    sfxText.setString("SFX: ON");
    sfxText.setScale(1.1f, 1.1f);
    sfxText.setPosition(centerX - sfxText.getGlobalBounds().width / 2.f, 280.f);

    // Volume Toggle Boxes
    musicButton.setSize(sf::Vector2f(musicText.getGlobalBounds().width + 40.f, musicText.getGlobalBounds().height + 30.f));
    musicButton.setFillColor(sf::Color(150, 150, 150, 180));
    musicButton.setOutlineThickness(2.f);
    musicButton.setOutlineColor(sf::Color::Black);
    musicButton.setPosition(musicText.getPosition().x - 20.f, musicText.getPosition().y - 15.f);

    sfxButton.setSize(sf::Vector2f(sfxText.getGlobalBounds().width + 40.f, sfxText.getGlobalBounds().height + 30.f));
    sfxButton.setFillColor(sf::Color(150, 150, 150, 180));
    sfxButton.setOutlineThickness(2.f);
    sfxButton.setOutlineColor(sf::Color::Black);
    sfxButton.setPosition(sfxText.getPosition().x - 20.f, sfxText.getPosition().y - 15.f);

    // Return to Menu
    returnButton.setFont(font);
    returnButton.setString("Return to Menu");
    returnButton.setCharacterSize(40);
    returnButton.setFillColor(sf::Color::White);
    returnButton.setPosition(window.getSize().x / 2.f - 140.f, 530.f);

    returnButtonBox.setFillColor(sf::Color(0, 0, 0, 150));
    returnButtonBox.setSize(sf::Vector2f(returnButton.getGlobalBounds().width + 40.f, returnButton.getGlobalBounds().height + 30.f));
    returnButtonBox.setPosition(returnButton.getPosition().x - 20.f, returnButton.getPosition().y - 15.f);


    loadHighScore();
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setPosition(10.f, 70.f);
    highScoreText.setString("High Score: " + std::to_string(highScore));

    // Hiển thị điểm cao nhất bên dưới nút Highest Score
    highScoreMenuText.setFont(font);
    highScoreMenuText.setCharacterSize(20);
    highScoreMenuText.setFillColor(sf::Color::Black);
    highScoreMenuText.setString("High Score: " + std::to_string(highScore));

    // Căn giữa bên dưới highScoreButtonBox
    float hsBoxCenter = highScoreButtonBox.getPosition().x + highScoreButtonBox.getSize().x / 2.f;
    float hsTextWidth = highScoreMenuText.getGlobalBounds().width;
    highScoreMenuText.setPosition(hsBoxCenter - hsTextWidth / 2.f,
        highScoreButtonBox.getPosition().y + highScoreButtonBox.getSize().y + 8.f);


    if (!clickBuffer.loadFromFile("assets/Sounds/click.wav"))
        std::cout << "❌ Không thể tải click.wav\n";
    else
        clickSound.setBuffer(clickBuffer);

    if (!shootBuffer.loadFromFile("assets/Sounds/shoot.wav"))
        std::cout << "❌ Không thể tải file shoot.wav\n";
    else {
        shootSound.setBuffer(shootBuffer);
        shootSound.setVolume(100.f);
    }

    if (!explosionBuffer.loadFromFile("assets/Sounds/explosion.wav"))
        std::cout << "❌ Không thể tải file explosion.wav\n";
    else {
        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(100.f);
    }

    if (!backgroundMusic.openFromFile("assets/Sounds/background.ogg")) {
        std::cout << "❌ Không thể mở background.ogg\n";
    }
    else {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(30.f);
        backgroundMusic.play();
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
                    clickSound.play();
                    gameState = GameState::Playing;
                    isRunning = true;
                    score = 0;
                    scoreText.setString("Score: 0");
                    bullets.clear();
                    enemies.clear();
                    waveNumber = 1;
                    enemyPerWave = 3;
                    enemySpawnedCount = 0;
                }
                else if (quitButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    window.close();
                }
                else if (moreButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gameState = GameState::MoreMenu;
                }
            }
            else if (gameState == GameState::MoreMenu)
            {
                if (tutorialButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gameState = GameState::Tutorial;
                }
                else if (volumeButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gameState = GameState::Volume;
                }
                else if (highScoreButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    showHighScoreText = !showHighScoreText;
                }
                
            }
            else if (gameState == GameState::Volume)
            {
                if (musicButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    isMusicOn = !isMusicOn;
                    musicText.setString("Music: " + std::string(isMusicOn ? "ON" : "OFF"));
                    if (isMusicOn)
                        backgroundMusic.play();
                    else
                        backgroundMusic.pause();
                }
                else if (sfxButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    isSFXOn = !isSFXOn;
                    sfxText.setString("SFX: " + std::string(isSFXOn ? "ON" : "OFF"));
                    shootSound.setVolume(isSFXOn ? 100.f : 0.f);
                    explosionSound.setVolume(isSFXOn ? 100.f : 0.f);
                    clickSound.setVolume(isSFXOn ? 100.f : 0.f);
                }
                else if (backButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gameState = GameState::MoreMenu;
                }
            }
            else if (gameState == GameState::GameOver)
            {
                if (retryButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gameState = GameState::Playing;
                    isRunning = true;
                    score = 0;
                    scoreText.setString("Score: 0");
                    bullets.clear();
                    enemies.clear();
                    waveNumber = 1;
                    enemyPerWave = 3;
                    enemySpawnedCount = 0;
                }
                else if (returnButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gameState = GameState::Menu;
                }
                else if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    window.close();
                }
            }
        }

        // ESC để quay lại Menu
        if (event.type == sf::Event::KeyPressed)
        {
            if (gameState == GameState::MoreMenu && event.key.code == sf::Keyboard::Escape)
            {
                clickSound.play();
                gameState = GameState::Menu;
            }
            else if (gameState == GameState::Tutorial && event.key.code == sf::Keyboard::Escape)
            {
                clickSound.play();
                gameState = GameState::MoreMenu;
            }
            else if (gameState == GameState::Volume && event.key.code == sf::Keyboard::Escape)
            {
                clickSound.play();
                gameState = GameState::MoreMenu;
            }
        }
    }

    if (isRunning)
    {
        player.handleInput();
    }
}

void Game::update(float dt)
{
    if (gameState == GameState::Menu || gameState == GameState::GameOver || gameState == GameState::MoreMenu)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        auto updateButtonHover = [&](sf::Text& button, sf::RectangleShape& box) {
            if (button.getGlobalBounds().contains(mousePos)) {
                button.setFillColor(sf::Color::Yellow);
                button.setScale(1.3f, 1.3f);
            }
            else {
                button.setFillColor(sf::Color::White);
                button.setScale(1.f, 1.f);
            }

            // Cập nhật kích thước và vị trí hộp bao
            box.setSize(sf::Vector2f(button.getGlobalBounds().width + 40.f, button.getGlobalBounds().height + 30.f));
            box.setPosition(button.getPosition().x - 20.f, button.getPosition().y - 15.f);
            };

        if (gameState == GameState::Menu) {
            updateButtonHover(playButton, playButtonBox);
            updateButtonHover(quitButton, quitButtonBox);
            updateButtonHover(moreButton, moreButtonBox);
        }
        else if (gameState == GameState::GameOver) {
            updateButtonHover(retryButton, retryButtonBox);
            updateButtonHover(exitButton, exitButtonBox);
            updateButtonHover(returnButton, returnButtonBox);
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
        }
        else if (gameState == GameState::MoreMenu) {
            updateButtonHover(tutorialButton, tutorialButtonBox);
            updateButtonHover(volumeButton, volumeButtonBox);
            updateButtonHover(highScoreButton, highScoreButtonBox);
            updateButtonHover(backButton, backButtonBox);
        }
        else if (gameState == GameState::Volume) {
            updateButtonHover(backButton, backButtonBox);
            updateButtonHover(sfxText, sfxButton);
            updateButtonHover(backButton, backButtonBox);
        }

        return;
    }

    if (!isRunning)
        return;

    waveText.setString("Wave: " + std::to_string(waveNumber));

    player.update(dt);

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

    auto& playerBullets = player.getBullets();
    for (auto b = playerBullets.begin(); b != playerBullets.end(); )
    {
        bool bulletErased = false;
        for (auto& enemy : enemies)
        {
            if (enemy.isHit(b->getBounds()))
            {
                b = playerBullets.erase(b);
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

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e)
        { return e.shouldBeRemoved(); }),
        enemies.end());

    if (enemies.empty() && enemySpawnedCount >= enemyPerWave)
    {
        waveNumber++;
        enemyPerWave += 2;
        enemySpawnedCount = 0;
        waveText.setString("Wave: " + std::to_string(waveNumber));
        player.setPosition(sf::Vector2f(100.f, 100.f));
    }

    sf::FloatRect playerBounds(player.getPosition().x, player.getPosition().y, 40.f, 40.f);
    for (auto& enemy : enemies)
    {
        if (enemy.isHit(playerBounds))
        {
            player.takeDamage(20);
            if (player.getHP() <= 0)
            {
                isRunning = false;
                gameState = GameState::GameOver;

                if (score > highScore)
                {
                    highScore = score;
                    saveHighScore();
                    highScoreText.setString("High Score: " + std::to_string(highScore));
                }
            }
            break;
        }
    }
}



void Game::render()
{
    window.clear();

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto handleHover = [&](sf::Text& text, sf::RectangleShape& box) {
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(sf::Color::Yellow);
            text.setScale(1.5f, 1.5f);
        }
        else {
            text.setFillColor(sf::Color::White);
            text.setScale(1.3f, 1.3f);
        }
        // Cập nhật lại vị trí box theo scale mới
        box.setSize(sf::Vector2f(text.getGlobalBounds().width + 40.f, text.getGlobalBounds().height + 30.f));
        box.setPosition(text.getPosition().x - 20.f, text.getPosition().y - 15.f);
        };

    if (gameState == GameState::Menu)
    {
        window.draw(menuBackgroundSprite);

        // Hover và vẽ các nút chính
        handleHover(playButton, playButtonBox);
        handleHover(quitButton, quitButtonBox);
        handleHover(moreButton, moreButtonBox);

        window.draw(playButtonBox);
        window.draw(quitButtonBox);
        window.draw(moreButtonBox);
        window.draw(playButton);
        window.draw(quitButton);
        window.draw(moreButton);
    }

    else if (gameState == GameState::MoreMenu)
    {
        window.draw(menuBackgroundSprite);

        // Hover và vẽ các nút phụ
        handleHover(tutorialButton, tutorialButtonBox);
        handleHover(volumeButton, volumeButtonBox);
        handleHover(highScoreButton, highScoreButtonBox);

        window.draw(tutorialButtonBox);
        window.draw(volumeButtonBox);
        window.draw(highScoreButtonBox);
        window.draw(tutorialButton);
        window.draw(volumeButton);
        window.draw(highScoreButton);
        if (showHighScoreText) {
            highScoreText.setString("Highest Score: " + std::to_string(highScore));
            window.draw(highScoreText);
        }
    }
    else if (gameState == GameState::Playing)
    {
        window.draw(backgroundSprite);
        player.draw(window);
        for (const auto& bullet : player.getBullets())
            bullet.draw(window);
        for (const auto& enemy : enemies)
            enemy.draw(window);
        window.draw(scoreText);
        window.draw(waveText);
        window.draw(highScoreText);
        if (!isRunning)
            window.draw(gameOverText);
    }
    else if (gameState == GameState::GameOver)
    {
        window.draw(menuBackgroundSprite);

        window.draw(gameOverText);
        window.draw(scoreText);

        handleHover(retryButton, retryButtonBox);
        handleHover(exitButton, exitButtonBox);

        window.draw(retryButtonBox);
        window.draw(exitButtonBox);
        window.draw(retryButton);
        window.draw(exitButton);
        window.draw(returnButtonBox);
        window.draw(returnButton);
    }
    else if (gameState == GameState::Tutorial)
    {
        window.draw(menuBackgroundSprite);
        window.draw(tutorialBox);
        window.draw(tutorialText);
        // Vẽ các phím
        window.draw(wasdSprite);
        window.draw(spaceSprite);
        window.draw(escSprite);
    }
    else if (gameState == GameState::Volume)
    {
        window.draw(menuBackgroundSprite);

        window.draw(musicButton);
        window.draw(musicText);
        window.draw(sfxButton);
        window.draw(sfxText);

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

void Game::loadHighScore() {
    std::ifstream input("highscore.txt");
    if (input.is_open()) {
        int loadedScore;
        if (input >> loadedScore && loadedScore >= 0 && loadedScore < 1000000) {
            highScore = loadedScore;
        }
        else {
            highScore = 0;
        }
        input.close();
    }
    else {
        highScore = 0;
    }
}


void Game::saveHighScore()
{
    std::ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << highScore;
        file.close();
    }
}