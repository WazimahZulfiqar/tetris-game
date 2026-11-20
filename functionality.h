
#pragma once

// Game logic functions
bool anamoly();
void rotation(bool& rotate);
void movement(int& delta_x);
void checkLines(int& linesCleared);

void fallingPiece(float& timer, float& delay,
    int& colorNum, bool& gameOver);

void spawnPiece(int type, int colorNum);
void hardDrop(int& colorNum, bool& gameOver);
