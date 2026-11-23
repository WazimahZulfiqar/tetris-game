
#include "functionality.h"
#include "utils.h"
#include "pieces.h"
#include <string>
#include <cstdlib>

// Simple replacement for std::min
static int minInt(int a, int b) {
    return (a < b) ? a : b;
}

// GAME LOGIC

bool anamoly() {
    for (int i = 0; i < 4; i++) {
        int x = point_1[i][0];
        int y = point_1[i][1];

        if (x < 0 || x >= N) return false;

        // Floor
        if (y >= M) return false;

        if (y >= 0 && gameGrid[y][x] != -1)
            return false;
    }

    return true;
}

void rotation(bool& rotate) {
    if (!rotate) return;

    for (int i = 0; i < 4; i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
    }

    int cx = point_1[1][0];
    int cy = point_1[1][1];

    for (int i = 0; i < 4; i++) {
        int dx = point_1[i][0] - cx;
        int dy = point_1[i][1] - cy;

        point_1[i][0] = cx + dy;
        point_1[i][1] = cy - dx;
    }

    int kicks[] = { 0, -1, 1, -2, 2 };
    bool ok = false;

    for (int k : kicks) {
        for (int i = 0; i < 4; i++)
            point_1[i][0] += k;

        if (anamoly()) {
            ok = true;
            break;
        }

        for (int i = 0; i < 4; i++)
            point_1[i][0] -= k;
    }

    // Also try shifting down by 1 if still invalid
    if (!ok) {
        for (int i = 0; i < 4; i++)
            point_1[i][1] += 1;

        if (!anamoly()) {
            // Restore on total failure
            for (int i = 0; i < 4; i++)
                point_1[i][1] -= 1;

            for (int i = 0; i < 4; i++) {
                point_1[i][0] = point_2[i][0];
                point_1[i][1] = point_2[i][1];
            }
        }
    }

    rotate = false;
}

void movement(int& delta_x) {
    if (delta_x == 0) return;

    for (int i = 0; i < 4; i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
        point_1[i][0] += delta_x;
    }

    if (!anamoly()) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0] = point_2[i][0];
            point_1[i][1] = point_2[i][1];
        }
    }

    delta_x = 0;
}

void checkLines(int& linesCleared) {
    linesCleared = 0;
    int k = M - 1;

    for (int i = M - 1; i >= 0; i--) {
        int count = 0;

        for (int j = 0; j < N; j++) {
            if (gameGrid[i][j] != -1)
                count++;
        }

        if (count == N) {
            linesCleared++;
        }
        else {
            for (int j = 0; j < N; j++)
                gameGrid[k][j] = gameGrid[i][j];

            k--;
        }
    }

    // Clear rows above the new top
    for (int i = k; i >= 0; i--) {
        for (int j = 0; j < N; j++)
            gameGrid[i][j] = -1;
    }
}

void spawnPiece(int type, int colorNum) {
    for (int i = 0; i < 4; i++) {
        point_1[i][0] = (BLOCKS[type][i] % 2) + (N / 2) - 1;
        point_1[i][1] = BLOCKS[type][i] / 2;
    }
}

void fallingPiece(float& timer, float& delay,
    int& colorNum, bool& gameOver) {
    if (timer <= delay) return;

    timer = 0;

    // Try to move piece one row down
    for (int i = 0; i < 4; i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
        point_1[i][1] += 1;
    }

    if (!anamoly()) {
        // Restore position
        for (int i = 0; i < 4; i++) {
            point_1[i][0] = point_2[i][0];
            point_1[i][1] = point_2[i][1];
        }

        // Lock the piece into the grid
        for (int i = 0; i < 4; i++) {
            int px = point_1[i][0];
            int py = point_1[i][1];

            if (py >= 0 && py < M && px >= 0 && px < N)
                gameGrid[py][px] = colorNum;
        }

        // Clear completed lines and update score
        int cleared = 0;
        checkLines(cleared);

        if (cleared > 0) {
            const int pts[] = { 0, 100, 300, 500, 800 };

            g_score += pts[minInt(cleared, 4)] * g_level;
            g_lines += cleared;
            g_level = g_lines / 10 + 1;
        }

        // Spawn the next piece
        colorNum = g_nextColor;
        spawnPiece(g_nextType, colorNum);

        // Prepare a new next piece
        g_nextType = rand() % 7;
        g_nextColor = PIECE_COLOR[g_nextType];

        // Game over check
        if (!anamoly())
            gameOver = true;
    }
}

void hardDrop(int& colorNum, bool& gameOver) {
    // Move piece all the way down
    while (true) {
        for (int i = 0; i < 4; i++)
            point_1[i][1] += 1;

        if (!anamoly()) {
            for (int i = 0; i < 4; i++)
                point_1[i][1] -= 1;

            break;
        }

        g_score += 2;
    }

    // Lock
    for (int i = 0; i < 4; i++) {
        int px = point_1[i][0];
        int py = point_1[i][1];

        if (py >= 0 && py < M && px >= 0 && px < N)
            gameGrid[py][px] = colorNum;
    }

    int cleared = 0;
    checkLines(cleared);

    if (cleared > 0) {
        const int pts[] = { 0, 100, 300, 500, 800 };

        g_score += pts[minInt(cleared, 4)] * g_level;
        g_lines += cleared;
        g_level = g_lines / 10 + 1;
    }

    colorNum = g_nextColor;
    spawnPiece(g_nextType, colorNum);

    g_nextType = rand() % 7;
    g_nextColor = PIECE_COLOR[g_nextType];

    if (!anamoly())
        gameOver = true;
}
