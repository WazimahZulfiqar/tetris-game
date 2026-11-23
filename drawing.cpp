
#include "drawing.h"
#include "utils.h"
#include "pieces.h"

// DRAWING FUNCTIONS

// Draw the background
void drawPolkaBg(sf::RenderWindow& win) {
    sf::RectangleShape bg({ (float)WIN_W, (float)WIN_H });
    bg.setFillColor(C_BG);
    win.draw(bg);

    sf::CircleShape dot(3.5f);
    dot.setFillColor(sf::Color(255, 255, 255, 160));

    for (float y = 0; y < WIN_H; y += 26.f) {
        for (float x = 0; x < WIN_W; x += 26.f) {
            dot.setPosition({ x, y });
            win.draw(dot);
        }
    }
}

// Draw a simple card
void drawCard(sf::RenderWindow& win,
    float x, float y, float w, float h) {

    sf::RectangleShape card({ w, h });
    card.setPosition({ x, y });
    card.setFillColor(C_WHITE_SOFT);
    card.setOutlineColor(C_BTN);
    card.setOutlineThickness(3.f);

    win.draw(card);
}

// Draw a button
void drawButton(sf::RenderWindow& win, sf::Font& font,
    const std::string& label,
    float x, float y, float w, float h) {

    sf::RectangleShape button({ w, h });
    button.setPosition({ x, y });
    button.setFillColor(C_BTN);

    win.draw(button);

    sf::Text text(font, label, 13);
    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setPosition({
        x + w / 2.f - bounds.size.x / 2.f - bounds.position.x,
        y + h / 2.f - bounds.size.y / 2.f - bounds.position.y
        });

    win.draw(text);
}

// Draw centered text
void drawTextC(sf::RenderWindow& win, sf::Font& font,
    const std::string& str, unsigned size,
    sf::Color col, float cx, float y) {

    sf::Text text(font, str, size);
    text.setFillColor(col);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setPosition({
        cx - bounds.size.x / 2.f - bounds.position.x,
        y
        });

    win.draw(text);
}

// Draw left-aligned text
void drawTextL(sf::RenderWindow& win, sf::Font& font,
    const std::string& str, unsigned size,
    sf::Color col, float x, float y) {

    sf::Text text(font, str, size);
    text.setFillColor(col);
    text.setPosition({ x, y });

    win.draw(text);
}

// Check if the mouse is inside a rectangle
bool mouseIn(sf::Vector2i mouse,
    float x, float y, float w, float h) {

    return mouse.x >= x && mouse.x <= x + w &&
        mouse.y >= y && mouse.y <= y + h;
}

// Draw the board and grid
void drawBoardFrame(sf::RenderWindow& win) {
    sf::RectangleShape board({ (float)BOARD_W, (float)BOARD_H });
    board.setPosition({ (float)BOARD_X, (float)BOARD_Y });
    board.setFillColor(C_BOARD);
    board.setOutlineColor(C_BORDER);
    board.setOutlineThickness(3.f);

    win.draw(board);

    // Vertical grid lines
    for (int col = 0; col <= N; col++) {
        sf::RectangleShape line({ 1.f, (float)BOARD_H });
        line.setPosition({ (float)(BOARD_X + col * CELL), (float)BOARD_Y });
        line.setFillColor(C_GRID);
        win.draw(line);
    }

    // Horizontal grid lines
    for (int row = 0; row <= M; row++) {
        sf::RectangleShape line({ (float)BOARD_W, 1.f });
        line.setPosition({ (float)BOARD_X, (float)(BOARD_Y + row * CELL) });
        line.setFillColor(C_GRID);
        win.draw(line);
    }
}

// Draw the ghost piece
void drawGhost(sf::RenderWindow& win, sf::Sprite& sprite) {
    int ghostX[4];
    int ghostY[4];

    // Copy the current piece position
    for (int i = 0; i < 4; i++) {
        ghostX[i] = point_1[i][0];
        ghostY[i] = point_1[i][1];
    }

    bool canMove = true;

    // Find the lowest possible position
    while (canMove) {
        for (int i = 0; i < 4; i++)
            ghostY[i]++;

        for (int i = 0; i < 4; i++) {
            int x = ghostX[i];
            int y = ghostY[i];

            if (x < 0 || x >= N || y >= M ||
                (y >= 0 && gameGrid[y][x] != -1)) {
                canMove = false;
                break;
            }
        }

        if (!canMove) {
            for (int i = 0; i < 4; i++)
                ghostY[i]--;
        }
    }

    // Check whether the ghost is at the current position
    bool samePosition = true;

    for (int i = 0; i < 4; i++) {
        if (ghostY[i] != point_1[i][1]) {
            samePosition = false;
            break;
        }
    }

    if (samePosition)
        return;

    // Draw the ghost blocks
    for (int i = 0; i < 4; i++) {
        if (ghostY[i] < 0)
            continue;

        sf::RectangleShape block({ (float)CELL, (float)CELL });
        block.setPosition({
            (float)(BOARD_X + ghostX[i] * CELL),
            (float)(BOARD_Y + ghostY[i] * CELL)
            });

        block.setFillColor(sf::Color(165, 201, 173, 100));
        block.setOutlineColor(C_BORDER);
        block.setOutlineThickness(1.f);

        win.draw(block);
    }
}

// Draw the next piece preview
void drawNextPiece(sf::RenderWindow& win, sf::Sprite& sprite,
    float boxX, float boxY, float boxW, float boxH) {

    const float blockSize = 24.f;

    float startX = boxX + (boxW - 2 * blockSize) / 2.f;
    float startY = boxY + (boxH - 2 * blockSize) / 2.f;

    for (int i = 0; i < 4; i++) {
        int col = BLOCKS[g_nextType][i] % 2;
        int row = BLOCKS[g_nextType][i] / 2;

        sprite.setTextureRect(
            sf::IntRect(
                { g_nextColor * CELL, 0 },
                { CELL, CELL }
            )
        );

        sprite.setScale({
            blockSize / (float)CELL,
            blockSize / (float)CELL
            });

        sprite.setPosition({
            startX + col * blockSize,
            startY + row * blockSize
            });

        win.draw(sprite);
    }

    // Restore the sprite scale
    sprite.setScale({ 1.f, 1.f });
}
