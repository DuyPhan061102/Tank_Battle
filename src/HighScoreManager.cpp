#include "HighScoreManager.h"
#include <fstream>
#include <ctime>
#include <algorithm>

HighScoreManager::HighScoreManager() {
    load();
}

void HighScoreManager::load() {
    scores.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    int score;
    std::string dateTime;
    while (file >> score) {
        std::getline(file, dateTime);
        if (!dateTime.empty() && dateTime[0] == ' ')
            dateTime.erase(0, 1);
        scores.push_back({ score, dateTime });
    }
    file.close();
    sortScores();
}

void HighScoreManager::save() {
    std::ofstream file(filePath);
    for (auto& s : scores) {
        file << s.score << " " << s.dateTime << "\n";
    }
    file.close();
}

void HighScoreManager::addScore(int score) {
    // Lấy thời gian hiện tại
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now); // Cú pháp chuẩn của localtime_s

    // Định dạng thời gian
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeInfo);

    // Thêm điểm mới
    scores.push_back({ score, buffer });
    sortScores();
    if (scores.size() > 5) scores.resize(5);
    save();
}

const std::vector<HighScoreEntry>& HighScoreManager::getScores() const {
    return scores;
}

void HighScoreManager::sortScores() {
    std::sort(scores.begin(), scores.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b) {
            return a.score > b.score;
        }
    );
}

void HighScoreManager::drawBox(sf::RenderWindow& window, sf::Font& font, sf::Vector2f pos) const {
    sf::RectangleShape box(sf::Vector2f(500, 400));
    box.setPosition(pos);
    box.setFillColor(sf::Color(0, 0, 0, 180));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2);
    window.draw(box);

    sf::Text title("HIGH SCORES", font, 28);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(pos.x + 150, pos.y + 10);
    window.draw(title);

    float startY = pos.y + 60;
    int rank = 1;
    for (auto& entry : scores) {
        sf::Text text(
            std::to_string(rank) + ". " +
            std::to_string(entry.score) + " - " +
            entry.dateTime,
            font, 28.5
        );
        text.setFillColor(sf::Color::White);
        text.setPosition(pos.x + 20, startY);
        window.draw(text);

        startY += 60;
        rank++;
    }
}
