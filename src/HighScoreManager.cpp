#include "HighScoreManager.h"
#include <fstream>      // Đọc/ghi file
#include <ctime>        // Lấy thời gian hệ thống
#include <algorithm>    // std::sort

HighScoreManager::HighScoreManager() {
    load(); // Khi khởi tạo, tự động load điểm từ file
}

void HighScoreManager::load() {
    scores.clear(); // Xóa dữ liệu cũ
    std::ifstream file(filePath);
    if (!file.is_open()) return; // Nếu không mở được file => bỏ qua

    int score;
    std::string dateTime;
    // Đọc dữ liệu từng dòng: "score dateTime"
    while (file >> score) {
        std::getline(file, dateTime); // Đọc phần thời gian sau điểm
        if (!dateTime.empty() && dateTime[0] == ' ')
            dateTime.erase(0, 1); // Xóa khoảng trắng dư ở đầu
        scores.push_back({ score, dateTime });
    }
    file.close();
    sortScores(); // Sắp xếp điểm giảm dần
}

void HighScoreManager::save() {
    std::ofstream file(filePath);
    // Ghi toàn bộ danh sách điểm ra file
    for (auto& s : scores) {
        file << s.score << " " << s.dateTime << "\n";
    }
    file.close();
}

void HighScoreManager::addScore(int score) {
    // Lấy thời gian hiện tại
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now); // Hàm an toàn hơn localtime

    // Chuyển thời gian sang dạng "YYYY-MM-DD HH:MM"
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeInfo);

    // Thêm điểm mới vào danh sách
    scores.push_back({ score, buffer });

    // Sắp xếp giảm dần
    sortScores();

    // Giữ tối đa 5 mục điểm cao nhất
    if (scores.size() > 5) scores.resize(5);

    // Lưu lại ra file
    save();
}

const std::vector<HighScoreEntry>& HighScoreManager::getScores() const {
    return scores; // Trả về danh sách để hiển thị (chỉ đọc)
}

void HighScoreManager::sortScores() {
    std::sort(scores.begin(), scores.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b) {
            return a.score > b.score; // So sánh giảm dần
        }
    );
}

void HighScoreManager::drawBox(sf::RenderWindow& window, sf::Font& font, sf::Vector2f pos) const {
    // Khung nền mờ
    sf::RectangleShape box(sf::Vector2f(500, 400));
    box.setPosition(pos);
    box.setFillColor(sf::Color(0, 0, 0, 180)); // Đen mờ
    box.setOutlineColor(sf::Color::White);     // Viền trắng
    box.setOutlineThickness(2);
    window.draw(box);

    // Tiêu đề
    sf::Text title("HIGH SCORES", font, 28);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(pos.x + 150, pos.y + 10);
    window.draw(title);

    // Vẽ danh sách điểm
    float startY = pos.y + 60; // Vị trí dòng đầu tiên
    int rank = 1;              // Hạng (1, 2, 3,...)
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

        startY += 60; // Xuống dòng
        rank++;
    }
}
