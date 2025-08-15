#pragma once
#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

// Cấu trúc lưu thông tin một mục điểm cao (high score)
struct HighScoreEntry {
    int score = 0;              // Điểm đạt được
    std::string dateTime;       // Ngày giờ đạt được điểm đó (dùng để hiển thị)
};

class HighScoreManager {
private:
    std::vector<HighScoreEntry> scores;    // Danh sách các điểm cao
    const std::string filePath = "highscore.txt"; // File lưu trữ điểm cao trên ổ đĩa

    // Hàm sắp xếp danh sách điểm cao (theo thứ tự giảm dần)
    void sortScores();

public:
    HighScoreManager();

    // Đọc điểm cao từ file (nếu file tồn tại)
    void load();

    // Ghi điểm cao hiện tại xuống file
    void save();

    // Thêm một điểm mới vào danh sách, sau đó sắp xếp và lưu
    void addScore(int score);

    // Trả về danh sách điểm cao (chỉ đọc, không cho chỉnh sửa trực tiếp)
    const std::vector<HighScoreEntry>& getScores() const;

    // Vẽ bảng điểm cao lên màn hình game
    // pos: vị trí của bảng trên cửa sổ
    void drawBox(sf::RenderWindow& window, sf::Font& font, sf::Vector2f pos) const;
};

#endif
