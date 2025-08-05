// Game.cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "Enemy.h"

Game::Game() : window(sf::VideoMode(800, 600), "Tank Battle"), gameState(GameState::Menu), isRunning(true)
{

    player.setPosition(playerSpawnPosition);

    player.setWalls(&walls);

    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(time(nullptr)));
    score = 0;
    loadHighScore();
    player.setWindow(&window);
    player.setShootSound(&shootSound);

    loadAssets();
    setupBackgrounds();
    setupFontsAndText();
    setupTutorial();
    setupUIButtons();
    setupAudio();
}

void Game::loadAssets()
{
    if (!backgroundTexture.loadFromFile("assets/Images/background.jpg"))
        std::cout << "❌ Không thể tải background.jpg\n";

    if (!menuBackgroundTexture.loadFromFile("assets/Images/menu_background.jpg"))
        std::cout << "❌ Không thể tải menu_background.jpg\n";

    if (!font.loadFromFile("assets/Fonts/Orbitron-Regular.ttf"))
        std::cout << "❌ Không thể tải font Roboto-Regular.ttf\n";

    if (!wasdTexture.loadFromFile("assets/Images/key_wasd.png"))
        std::cout << "Không thể tải ảnh WASD\n";

    if (!spaceTexture.loadFromFile("assets/Images/key_space.png"))
        std::cout << "Không thể tải ảnh SPACE\n";

    if (!escTexture.loadFromFile("assets/Images/key_esc.png"))
        std::cout << "Không thể tải ảnh ESC\n";

    if (!wallTexture.loadFromFile("assets/Images/wall.png"))
        std::cout << "❌ Không thể tải wall.png\n";

    if (!strongWallTexture.loadFromFile("assets/Images/strong_wall.png"))
        std::cout << "❌ Không thể tải strong_wall.png\n";

    if (!playerTexture.loadFromFile("assets/Images/tank1.png"))
        std::cout << "❌ Không thể tải player_tank.png\n";
}

void Game::setupBackgrounds()
{
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height);

    menuBackgroundSprite.setTexture(menuBackgroundTexture);
    menuBackgroundSprite.setScale(
        window.getSize().x / menuBackgroundSprite.getLocalBounds().width,
        window.getSize().y / menuBackgroundSprite.getLocalBounds().height);
}

void Game::setupTutorial()
{
    wasdSprite.setTexture(wasdTexture);
    wasdSprite.setPosition(60.f, 300.f);
    wasdSprite.setScale(0.4f, 0.4f);

    spaceSprite.setTexture(spaceTexture);
    spaceSprite.setPosition(250.f, 320.f);
    spaceSprite.setScale(0.3f, 0.3f);

    escSprite.setTexture(escTexture);
    escSprite.setPosition(550.f, 320.f);
    escSprite.setScale(0.5f, 0.5f);

    tutorialBox.setSize(sf::Vector2f(700.f, 450.f));
    tutorialBox.setFillColor(sf::Color(100, 100, 100, 200));
    tutorialBox.setOutlineThickness(3.f);
    tutorialBox.setOutlineColor(sf::Color::Black);
    tutorialBox.setPosition(50.f, 75.f);

    tutorialText.setFont(font);
    tutorialText.setCharacterSize(22);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setString("Instructions:\n\n- Use W / A / S / D to move the tank.\n- Press SPACE to shoot.\n- Press ESC to return in Menu.");
    tutorialText.setPosition(80.f, 100.f);
}

/*
    if (!wallTexture.loadFromFile("assets/Images/wall.png"))
        std::cout << "❌ Không thể tải wall.png\n";

    if (!strongWallTexture.loadFromFile("assets/Images/strong_wall.png"))
        std::cout << "❌ Không thể tải strong_wall.png\n";
    createMaze();*/

void Game::setupFontsAndText()
{

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

    gameTitle.setFont(font);
    gameTitle.setString("TANK BATTLE");
    gameTitle.setCharacterSize(64);
    gameTitle.setFillColor(sf::Color::Red);

    sf::FloatRect titleBounds = gameTitle.getLocalBounds();
    gameTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    gameTitle.setPosition(window.getSize().x / 2.f, 70.f);

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(230.f, 90.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setPosition(10.f, 70.f);
    highScoreText.setString("High Score: " + std::to_string(highScore));

    highScoreMenuText.setFont(font);
    highScoreMenuText.setCharacterSize(20);
    highScoreMenuText.setFillColor(sf::Color::Black);
    highScoreMenuText.setString("High Score: " + std::to_string(highScore));
}

void Game::setupUIButtons()
{
    float centerX = window.getSize().x / 2.f;
    auto setupButtonFull = [&](sf::Text &text, sf::RectangleShape &box, const std::string &str, unsigned int charSize, float y)
    {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(charSize);
        text.setFillColor(sf::Color::White);
        text.setScale(1.f, 1.f);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
        text.setPosition(centerX, y);

        float paddingX = 40.f;
        float paddingY = 20.f;
        box.setSize(sf::Vector2f(textBounds.width + paddingX, textBounds.height + paddingY));
        box.setOrigin(box.getSize().x / 2.f, box.getSize().y / 2.f);
        box.setPosition(text.getPosition());
        box.setFillColor(sf::Color(150, 150, 150, 180));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::Black);
    };

    setupButtonFull(playButton, playButtonBox, "PLAY", 40, 230.f);
    setupButtonFull(moreButton, moreButtonBox, "MORE", 40, 330.f);
    setupButtonFull(quitButton, quitButtonBox, "QUIT", 40, 430.f);

    setupButtonFull(retryButton, retryButtonBox, "RETRY", 40, 230.f);
    setupButtonFull(exitButton, exitButtonBox, "EXIT", 40, 330.f);

    setupButtonFull(tutorialButton, tutorialButtonBox, "TUTORIAL", 36, 200.f);
    setupButtonFull(volumeButton, volumeButtonBox, "VOLUME", 36, 300.f);
    setupButtonFull(highScoreButton, highScoreButtonBox, "HIGHSCORE", 36, 400.f);

    setupButtonFull(musicText, musicButton, "MUSIC: ON", 45, 220.f);
    setupButtonFull(sfxText, sfxButton, "SFX: ON", 45, 320.f);

    setupButtonFull(returnButton, returnButtonBox, "RETURN TO MENU", 40, 430.f);

    float hsBoxCenter = highScoreButtonBox.getPosition().x + highScoreButtonBox.getSize().x / 2.f;
    float hsTextWidth = highScoreMenuText.getGlobalBounds().width;
    highScoreMenuText.setPosition(hsBoxCenter - hsTextWidth / 2.f,
                                  highScoreButtonBox.getPosition().y + highScoreButtonBox.getSize().y + 8.f);
}

void Game::setupButtonText(sf::Text &text, const std::string &str, unsigned int charSize)
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    text.setScale(1.f, 1.f);
}

void Game::setupAudio()
{
    if (!clickBuffer.loadFromFile("assets/Sounds/click.wav"))
        std::cout << "❌ Không thể tải click.wav\n";
    else
        clickSound.setBuffer(clickBuffer);

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

    if (!backgroundMusic.openFromFile("assets/Sounds/background.ogg"))
    {
        std::cout << "❌ Không thể mở background.ogg\n";
    }
    else
    {
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

                    // Reset player
                    player = PlayerTank();
                    player.setWalls(&walls);
                    player.setWindow(&window);
                    player.setShootSound(&shootSound);
                    player.setPosition(playerSpawnPosition);
                    player.resetHP(); // <-- Thêm dòng này để hồi máu player
                    player.setTexture(&playerTexture); // <-- Thêm dòng này

                    createMaze();
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
                    musicText.setString("MUSIC: " + std::string(isMusicOn ? "ON" : "OFF"));
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

                    // 👉 Thêm các dòng sau để khởi tạo lại player và map:
                    player = PlayerTank();
                    player.setWalls(&walls);
                    player.setWindow(&window);
                    player.setShootSound(&shootSound);
                    player.setPosition(playerSpawnPosition); // ✅ dùng spawnPoint từ map
                    player.resetHP();
                    player.setTexture(&playerTexture); // <-- Thêm dòng này

                    createMaze();
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

void Game::updateMenuButtonHovers()
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto updateButtonHover = [&](sf::Text &button, sf::RectangleShape &box)
    {
        if (button.getGlobalBounds().contains(mousePos))
        {
            button.setFillColor(sf::Color::Yellow);
        }
        else
        {
            button.setFillColor(sf::Color::White);
        }
    };

    switch (gameState)
    {
    case GameState::Menu:
        updateButtonHover(playButton, playButtonBox);
        updateButtonHover(moreButton, moreButtonBox);
        updateButtonHover(quitButton, quitButtonBox);
        break;

    case GameState::GameOver:
        updateButtonHover(retryButton, retryButtonBox);
        updateButtonHover(exitButton, exitButtonBox);
        updateButtonHover(returnButton, returnButtonBox);
        break;

    case GameState::MoreMenu:
        updateButtonHover(tutorialButton, tutorialButtonBox);
        updateButtonHover(volumeButton, volumeButtonBox);
        updateButtonHover(highScoreButton, highScoreButtonBox);
        updateButtonHover(backButton, backButtonBox);
        break;

    case GameState::Volume:
        updateButtonHover(musicText, musicButton);
        updateButtonHover(sfxText, sfxButton);
        updateButtonHover(backButton, backButtonBox);
        break;

    default:
        break;
    }
}

void Game::update(float dt)
{
    if (gameState == GameState::Menu ||
        gameState == GameState::GameOver ||
        gameState == GameState::MoreMenu ||
        gameState == GameState::Volume)
    {
        updateMenuButtonHovers();
        return;
    }

    if (!isRunning)
        return;

    waveText.setString("Wave: " + std::to_string(waveNumber));

    player.update(dt);
    // Xóa tường
    walls.erase(
        std::remove_if(walls.begin(), walls.end(),
                       [](const Wall &w)
                       { return w.isDestroyed(); }),
        walls.end());

    // Kiểm tra máu sau khi cập nhật
    if (player.getHP() <= 0 && gameState == GameState::Playing)
    {
        isRunning = false;
        gameState = GameState::GameOver;

        if (score > highScore)
        {
            highScore = score;
            saveHighScore();
            highScoreText.setString("High Score: " + std::to_string(highScore));
        }
        return; // Không cần xử lý gì thêm
    }
    if (enemySpawnClock.getElapsedTime().asSeconds() > 3.f)
    {
        spawnEnemy();
        enemySpawnClock.restart();
    }

    for (auto &enemy : enemies)
        enemy->update(dt);
    for (auto &enemy : enemies)
    {
        sf::FloatRect enemyBounds(enemy->getPosition().x, enemy->getPosition().y, 40.f, 40.f);

        for (const Wall &wall : walls)
        {
            if (wall.getBounds().intersects(enemyBounds))
            {
                // Cách đơn giản: quay đầu
                enemy->setSpeed(-enemy->getSpeed());

                // Hoặc đổi hướng ngẫu nhiên:
                // int dir = rand() % 4; ... (như trong Enemy constructor)

                break; // Xử lý xong một tường là đủ
            }
        }
    }

    for (auto &bullet : bullets)
        bullet.update(dt);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [this](const Bullet &b)
                                 {
                                     return b.isOffScreen(window);
                                 }),
                  bullets.end());

    auto &playerBullets = player.getBullets();
    for (auto b = playerBullets.begin(); b != playerBullets.end();)
    {
        bool bulletErased = false;
        for (auto &enemy : enemies)
        {
            if (enemy->isHit(b->getBounds()))
            {
                b = playerBullets.erase(b);
                explosionSound.play();
                enemy->markToRemove();
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
                       [](const std::unique_ptr<Enemy> &e)
                       { return e->shouldBeRemoved(); }),
        enemies.end());

    if (enemies.empty() && enemySpawnedCount >= enemyPerWave)
    {
        waveNumber++;
        enemyPerWave += 2;
        enemySpawnedCount = 0;
        waveText.setString("Wave: " + std::to_string(waveNumber));
        player.setPosition(playerSpawnPosition); // Spawn theo spawn point
        createMaze();
    }

    // Va chạm enemy với player
    sf::FloatRect playerBounds(player.getPosition().x, player.getPosition().y, 40.f, 40.f);
    for (auto &enemy : enemies)
    {
        if (enemy->isHit(playerBounds))
        {
            player.takeDamage(20); // hoặc giá trị bạn muốn, nên chọn 20 cho hợp lý
        }
    }
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

}

void Game::render()
{
    window.clear();

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto handleHover = [&](sf::Text &text, sf::RectangleShape &box)
    {
        if (text.getGlobalBounds().contains(mousePos))
            text.setFillColor(sf::Color::Yellow);
        else
            text.setFillColor(sf::Color::White);
    };

    if (gameState == GameState::Menu)
    {
        window.draw(menuBackgroundSprite);
        window.draw(gameTitle);
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
    else if (gameState == GameState::Playing)
    {
        window.draw(backgroundSprite);
        for (const Wall &wall : walls)
            wall.draw(window);
        player.draw(window);
        for (const auto &enemy : enemies)
            enemy->draw(window);
        for (const auto &bullet : player.getBullets())
            bullet.draw(window);
        window.draw(scoreText);
        window.draw(waveText);
        window.draw(highScoreText);
        if (!isRunning)
            window.draw(gameOverText);
    }
    else if (gameState == GameState::MoreMenu)
    {
        window.draw(menuBackgroundSprite);
        handleHover(tutorialButton, tutorialButtonBox);
        handleHover(volumeButton, volumeButtonBox);
        handleHover(highScoreButton, highScoreButtonBox);
        window.draw(tutorialButtonBox);
        window.draw(volumeButtonBox);
        window.draw(highScoreButtonBox);
        window.draw(tutorialButton);
        window.draw(volumeButton);
        window.draw(highScoreButton);
        if (showHighScoreText)
        {
            highScoreText.setString("Highest Score: " + std::to_string(highScore));
            window.draw(highScoreText);
        }
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
    if (enemySpawnedCount >= enemyPerWave || enemySpawnPoints.empty())
        return;

    // Lọc các vị trí còn spawn được
    std::vector<int> availableIndexes;
    for (int i = 0; i < enemySpawnCounts.size(); ++i)
    {
        if (enemySpawnCounts[i] < maxEnemiesPerSpawn)
            availableIndexes.push_back(i);
    }

    if (availableIndexes.empty())
        return; // Không còn chỗ spawn

    // Chọn ngẫu nhiên từ các vị trí còn trống
    int chosen = availableIndexes[rand() % availableIndexes.size()];
    sf::Vector2f spawnPos = enemySpawnPoints[chosen];

    // Sử dụng unique_ptr cho Enemy (theo nhánh dev)
    auto e = std::make_unique<Enemy>(spawnPos.x, spawnPos.y);
    e->setSpeed(50.f * std::pow(1.35f, waveNumber));
    enemies.push_back(std::move(e));
    enemySpawnCounts[chosen]++; // tăng số enemy ở vị trí này
    enemySpawnedCount++;
}

void Game::loadHighScore()
{
    std::ifstream input("highscore.txt");
    if (input.is_open())
    {
        int loadedScore;
        if (input >> loadedScore && loadedScore >= 0 && loadedScore < 1000000)
        {
            highScore = loadedScore;
        }
        else
        {
            highScore = 0;
        }
        input.close();
    }
    else
    {
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
void Game::createMaze()
{
    std::ifstream file("assets/Maps/maze.txt");
    if (!file.is_open())
    {
        std::cerr << "❌ Không thể mở maze.txt\n";
        return;
    }

    walls.clear();
    enemySpawnPoints.clear();
    enemySpawnCounts.clear();

    std::string line;
    int row = 0;
    const int tileSize = 40;

    // 🔸 Làm tường nhỏ hơn tileSize
    const float wallSizeRatio = 0.75f; // 👈 Giảm kích thước (có thể điều chỉnh: 0.5f, 0.7f,...)
    const sf::Vector2f wallSize(tileSize * wallSizeRatio, tileSize * wallSizeRatio);
    const sf::Vector2f wallOffset((tileSize - wallSize.x) / 2.f, (tileSize - wallSize.y) / 2.f);

    while (std::getline(file, line))
    {
        for (int col = 0; col < line.size(); ++col)
        {
            char ch = line[col];
            sf::Vector2f pos(col * tileSize, row * tileSize);

            switch (ch)
            {
            case '#':
            {
                Wall w(pos, sf::Vector2f(tileSize - 4, tileSize - 4), 15);
                w.setTexture(&wallTexture); // Gán ảnh cho tường thường
                walls.push_back(w);
                break;
            }
            case '@':
            {
                Wall w(pos, sf::Vector2f(tileSize - 4, tileSize - 4), 99999);
                w.setTexture(&strongWallTexture); // Gán ảnh cho tường bền
                walls.push_back(w);
                break;
            }

            case 'S':
                playerSpawnPosition = pos;
                break;
            case 'E':
                enemySpawnPoints.push_back(pos);
                enemySpawnCounts.push_back(0);
                break;
            case '.':
                break;
            default:
                break;
            }
        }
        row++;
    }

    file.close();

    player.setWalls(&walls);
    player.setPosition(playerSpawnPosition);
}