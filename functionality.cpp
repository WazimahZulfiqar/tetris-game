
#include "functionality.h"
#include "utils.h"
#include "pieces.h"
#include <cstdlib>

// GAME LOGIC

// Check whether the current piece is in a valid position
bool anamoly() {
    for (int i = 0; i < 4; i++) {
        int x = point_1[i][0];
        int y = point_1[i][1];

        if (x < 0 || x >= N || y >= M)
            return false;

        if (y >= 0 && gameGrid[y][x] != -1)
            return false;
    }

    return true;
}

// Rotate the current piece
void rotation(bool& rotate) {
    if (!rotate)
        return;

    // Save the current position
    for (int i = 0; i < 4; i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
    }

    // Use the second block as the rotation center
    int cx = point_1[1][0];
    int cy = point_1[1][1];

    for (int i = 0; i < 4; i++) {
        int dx = point_1[i][0] - cx;
        int dy = point_1[i][1] - cy;

        point_1[i][0] = cx + dy;
        point_1[i][1] = cy - dx;
    }

    // Restore the piece if rotation is invalid
    if (!anamoly()) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0] = point_2[i][0];
            point_1[i][1] = point_2[i][1];
        }
    }

    rotate = false;
}

// Move the piece horizontally
void movement(int& delta_x) {
    if (delta_x == 0)
        return;

    for (int i = 0; i < 4; i++)
        point_1[i][0] += delta_x;

    // Undo movement if it causes a collision
    if (!anamoly()) {
        for (int i = 0; i < 4; i++)
            point_1[i][0] -= delta_x;
    }

    delta_x = 0;
}

// Clear completed rows
void checkLines(int& linesCleared) {
    linesCleared = 0;

    int newRow = M - 1;

    for (int row = M - 1; row >= 0; row--) {
        int count = 0;

        // Count occupied cells in this row
        for (int col = 0; col < N; col++) {
            if (gameGrid[row][col] != -1)
                count++;
        }

        // Skip a full row
        if (count == N) {
            linesCleared++;
        }
        else {
            // Move non-full row downward
            for (int col = 0; col < N; col++)
                gameGrid[newRow][col] = gameGrid[row][col];

            newRow--;
        }
    }

    // Clear the remaining top rows
    for (int row = newRow; row >= 0; row--) {
        for (int col = 0; col < N; col++)
            gameGrid[row][col] = -1;
    }
}

// Spawn a new piece at the top of the board
void spawnPiece(int type, int colorNum) {
    (void)colorNum; // Color is handled separately

    for (int i = 0; i < 4; i++) {
        point_1[i][0] = (BLOCKS[type][i] % 2) + (N / 2) - 1;
        point_1[i][1] = BLOCKS[type][i] / 2;
    }
}

// Make the piece fall automatically
void fallingPiece(float& timer, float& delay,
    int& colorNum, bool& gameOver) {

    if (timer <= delay)
        return;

    timer = 0;

    // Move the piece down
    for (int i = 0; i < 4; i++)
        point_1[i][1]++;

    // If the piece cannot move further
    if (!anamoly()) {

        // Move it back to its last valid position
        for (int i = 0; i < 4; i++)
            point_1[i][1]--;

        // Lock the piece into the board
        for (int i = 0; i < 4; i++) {
            int x = point_1[i][0];
            int y = point_1[i][1];

            if (y >= 0 && y < M && x >= 0 && x < N)
                gameGrid[y][x] = colorNum;
        }

        // Clear rows and update score
        int cleared = 0;
        checkLines(cleared);

        if (cleared > 0) {
            int points[] = { 0, 100, 300, 500, 800 };

            if (cleared > 4)
                cleared = 4;

            g_score += points[cleared] * g_level;
            g_lines += cleared;
            g_level = g_lines / 10 + 1;
        }

        // Spawn the next piece
        colorNum = g_nextColor;
        spawnPiece(g_nextType, colorNum);

        // Choose another piece for the next preview
        g_nextType = rand() % 7;
        g_nextColor = PIECE_COLOR[g_nextType];

        // Check whether the new piece can spawn
        if (!anamoly())
            gameOver = true;
    }
}

// Drop the piece straight down
void hardDrop(int& colorNum, bool& gameOver) {

    // Keep moving down until the position becomes invalid
    while (true) {
        for (int i = 0; i < 4; i++)
            point_1[i][1]++;

        if (!anamoly()) {
            // Undo the last downward move
            for (int i = 0; i < 4; i++)
                point_1[i][1]--;

            break;
        }

        g_score += 2;
    }

    // Lock the piece into the board
    for (int i = 0; i < 4; i++) {
        int x = point_1[i][0];
        int y = point_1[i][1];

        if (y >= 0 && y < M && x >= 0 && x < N)
            gameGrid[y][x] = colorNum;
    }

    // Clear rows and update score
    int cleared = 0;
    checkLines(cleared);

    if (cleared > 0) {
        int points[] = { 0, 100, 300, 500, 800 };

        if (cleared > 4)
            cleared = 4;

        g_score += points[cleared] * g_level;
        g_lines += cleared;
        g_level = g_lines / 10 + 1;
    }

    // Spawn the next piece
    colorNum = g_nextColor;
    spawnPiece(g_nextType, colorNum);

    // Select the next preview piece
    g_nextType = rand() % 7;
    g_nextColor = PIECE_COLOR[g_nextType];

    // Check whether the next piece can spawn
    if (!anamoly())
        gameOver = true;
}
