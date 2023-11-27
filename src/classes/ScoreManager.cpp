#include "ScoreManager.h"

ScoreManager::ScoreManager() {
    this->score = 0;
}

int ScoreManager::getScore() {
    return this->score;
}

void ScoreManager::setScore(int value) {
    this->score = value;
}

int ScoreManager::Add(int add) {
    this->score += add;
}