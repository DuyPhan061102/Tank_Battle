#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "Enemy.h"

Game::Game() : window(sf::VideoMode(800, 600), "Tank Battle"), gameState(GameState::Menu), isRunning(true) 
{
    createMaze();
    player.setPosition(playerSpawnPosition);

    player.setWalls(&walls);

    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(time(nullptr)));
    score = 0;
    player.setWindow(&window);
    player.setShootSound(&shootSound);

    if (!backgroundTexture.loadFromFile("assets/Images/background.jpg"))
        std::cout << "❌ Không thể tải background.jpg\n";
    else
    {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            window.getSize().x / backgroundSprite.getLocalBounds().width,
            window.getSize().y / backgroundSprite.getLocalBounds().height);
    }

    if (!menuBackgroundTexture.loadFromFile("assets/Images/menu_background.jpg"))
        std::cout << "❌ Không thể tải menu_background.jpg\n";
    else
    {
        menuBackgroundSprite.setTexture(menuBackgroundTexture);
        menuBackgroundSprite.setScale(
            window.getSize().x / menuBackgroundSprite.getLocalBounds().width,
            window.getSize().y / menuBackgroundSprite.getLocalBounds().height);
    }

    if (!font.loadFromFile("assets/Fonts/arial.ttf"))
        std::cout << "❌ Không thể tải font arial.ttf\n";

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

    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(36);
    playButton.setFillColor(sf::Color::Black);
    playButton.setPosition(centerX - playButton.getGlobalBounds().width / 2.f, 250.f);

    quitButton.setFont(font);
    quitButton.setString("Quit");
    quitButton.setCharacterSize(36);
    quitButton.setFillColor(sf::Color::Black);
    quitButton.setPosition(centerX - quitButton.getGlobalBounds().width / 2.f, 320.f);

    retryButton.setFont(font);
    retryButton.setString("Retry");
    retryButton.setCharacterSize(36);
    retryButton.setFillColor(sf::Color::Black);
    retryButton.setPosition(centerX - retryButton.getGlobalBounds().width / 2.f, 250.f);

    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(36);
    exitButton.setFillColor(sf::Color::Black);
    exitButton.setPosition(centerX - exitButton.getGlobalBounds().width / 2.f, 320.f);

    loadHighScore();
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setPosition(10.f, 70.f);
    highScoreText.setString("High Score: " + std::to_string(highScore));

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
        backgroundMusic.setLoop(true);   // lặp lại vô hạn
        backgroundMusic.setVolume(30.f); // âm lượng (0 - 100)
        backgroundMusic.play();          // phát nhạc
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
                    waveNumber = 1;
                    enemyPerWave = 3;
                    enemySpawnedCount = 0;
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
    waveNumber = 1;
    enemyPerWave = 3;
    enemySpawnedCount = 0;

    // 👉 Thêm các dòng sau:
player = PlayerTank();
player.setWalls(&walls);
player.setWindow(&window);
player.setShootSound(&shootSound);
player.setPosition(playerSpawnPosition);  // ✅ dùng spawnPoint từ map
createMaze();
                 }

                else if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }
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

    waveText.setString("Wave: " + std::to_string(waveNumber));

    // Spawn enemy mỗi 3 giây
    player.update(dt);
    //Xóa tường
     walls.erase(
    std::remove_if(walls.begin(), walls.end(),
                   [](const Wall& w) { return w.isDestroyed(); }),
    walls.end());


// Kiểm tra máu sau khi cập nhật
if (player.getHP() <= 0 && gameState == GameState::Playing) {
    isRunning = false;
    gameState = GameState::GameOver;

    if (score > highScore) {
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
        enemy.update(dt);
    for (auto& enemy : enemies) {
    sf::FloatRect enemyBounds(enemy.getPosition().x, enemy.getPosition().y, 40.f, 40.f);

    for (const Wall& wall : walls) {
        if (wall.getBounds().intersects(enemyBounds)) {
            // Cách đơn giản: quay đầu
            enemy.setSpeed(-enemy.getSpeed());

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

    // Xử lý va chạm đạn <-> enemy
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

    // Xóa enemy đã chết
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const Enemy &e)
                       { return e.shouldBeRemoved(); }),
        enemies.end());

    // Nếu đã spawn đủ enemy và hiện tại đã clear hết -> tăng wave mới
    if (enemies.empty() && enemySpawnedCount >= enemyPerWave)
    {
        waveNumber++;
        enemyPerWave += 2;
        enemySpawnedCount = 0;
        waveText.setString("Wave: " + std::to_string(waveNumber));

        player.setPosition(playerSpawnPosition);// Spawn theo s
        createMaze();
    }

    // Va chạm enemy với player
   sf::FloatRect playerBounds(player.getPosition().x, player.getPosition().y, 40.f, 40.f);
    for (auto &enemy : enemies)
    {
        if (enemy.isHit(playerBounds))
        player.takeDamage(50);
        {

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

    if (gameState == GameState::Menu)
    {
        window.draw(menuBackgroundSprite);
        

        playButton.setFillColor(sf::Color::Black);
        quitButton.setFillColor(sf::Color::Black);

        // Lấy vị trí chuột
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Hover màu vàng
        if (playButton.getGlobalBounds().contains(mousePos))
            playButton.setFillColor(sf::Color::Yellow);
        if (quitButton.getGlobalBounds().contains(mousePos))
            quitButton.setFillColor(sf::Color::Yellow);

        // Vẽ nút
        window.draw(playButton);
        window.draw(quitButton);
    }
    else
    {
        window.draw(backgroundSprite);

         for (const Wall& wall : walls)
           wall.draw(window);

        if (gameState == GameState::Playing)
        {
            player.draw(window);
            /* for (const auto &bullet : bullets)
                 bullet.draw(window);*/
            for (const auto &bullet : player.getBullets())
                bullet.draw(window);
            for (const auto &enemy : enemies)
                enemy.draw(window);
            window.draw(scoreText);
            window.draw(waveText);
            window.draw(highScoreText);
            if (!isRunning)
                window.draw(gameOverText);
        }
        else if (gameState == GameState::GameOver)
        {
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(retryButton);
            window.draw(exitButton);
        }
    }

    window.display();
}

void Game::spawnEnemy()
{
    if (enemySpawnedCount >= enemyPerWave || enemySpawnPoints.empty())
        return;

    // Lọc các vị trí còn spawn được
    std::vector<int> availableIndexes;
    for (int i = 0; i < enemySpawnCounts.size(); ++i) {
        if (enemySpawnCounts[i] < maxEnemiesPerSpawn)
            availableIndexes.push_back(i);
    }

    if (availableIndexes.empty()) return; // Không còn chỗ spawn

    // Chọn ngẫu nhiên từ các vị trí còn trống
    int chosen = availableIndexes[rand() % availableIndexes.size()];
    sf::Vector2f spawnPos = enemySpawnPoints[chosen];

    Enemy e(spawnPos.x, spawnPos.y);
    e.setSpeed(50.f * std::pow(1.35f, waveNumber));

    enemies.push_back(e);
    enemySpawnCounts[chosen]++; // tăng số enemy ở vị trí này
    enemySpawnedCount++;
}



void Game::loadHighScore()
{
    std::ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> highScore;
        file.close();
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
void Game::createMaze() {
    std::ifstream file("/Users/macos/Desktop/Tank_Battle/assets/Maps/maze.txt");
    if (!file.is_open()) {
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

    while (std::getline(file, line)) {
        for (int col = 0; col < line.size(); ++col) {
            char ch = line[col];
            sf::Vector2f pos(col * tileSize, row * tileSize);

            switch (ch) {
                case '#':
                    walls.emplace_back(pos + wallOffset, wallSize, 10);
                    break;
                case '@':
                    walls.emplace_back(pos + wallOffset, wallSize, 20);
                    break;
                case 'S':
                    playerSpawnPosition = pos;
                    break;
                case 'E':
                    if (enemySpawnPoints.size() < 5) {
                        enemySpawnPoints.push_back(pos);
                        enemySpawnCounts.push_back(0);
                    }
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
