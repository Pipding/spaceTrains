#pragma once

class ScoreManager {
private:

    int score;

public:

    ScoreManager();

    int getScore();
    void setScore(int value);

    int Add(int add);

};