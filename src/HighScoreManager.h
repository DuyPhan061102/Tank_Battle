#pragma once
#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct HighScoreEntry {
    int score = 0;
    std::string dateTime;
};

class HighScoreManager {
private:
    std::vector<HighScoreEntry> scores;
    const std::string filePath = "highscore.txt";

    void sortScores();

public:
    HighScoreManager();

    void load();
    void save();
    void addScore(int score);
    const std::vector<HighScoreEntry>& getScores() const;

    void drawBox(sf::RenderWindow& window, sf::Font& font, sf::Vector2f pos) const;
};

#endif
