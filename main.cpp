
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>

#include "utils.h"
#include "pieces.h"
#include "functionality.h"

using namespace sf;

// Seven piece colors
Color pieceColors[8] = {
    Color::Black,
    Color(60, 180, 210),    // I - Cyan
    Color(220, 90, 90),     // Z - Red
    Color(100, 180, 110),   // S - Green
    Color(160, 110, 210),   // T - Purple
    Color(230, 160, 80),    // L - Orange
    Color(90, 120, 210),    // J - Blue
    Color(230, 200, 90)     // O - Yellow
};

// Function to draw text
void drawGameText(RenderWindow& window, Font& font,
    std::string message, unsigned int size,
    Color color, float x, float y)
{
    Text text(font, message, size);

    text.setFillColor(color);
    text.setPosition({ x, y });

    window.draw(text);
}

// Function to draw a panel
void drawPanel(RenderWindow& window,
    float x, float y, float width, float height)
{
    RectangleShape panel({ width, height });

    panel.setPosition({ x, y });
    panel.setFillColor(Color(246, 250, 241));
    panel.setOutlineColor(Color(125, 165, 135));
    panel.setOutlineThickness(2.f);

    window.draw(panel);
}

// Function to draw next piece
void drawNext(RenderWindow& window, int type, int color)
{
    int minX = 10;
    int minY = 10;

    for (int i = 0; i < 4; i++)
    {
        int x = BLOCKS[type][i] % 2;
        int y = BLOCKS[type][i] / 2;

        if (x < minX)
            minX = x;

        if (y < minY)
            minY = y;
    }

    for (int i = 0; i < 4; i++)
    {
        int x = BLOCKS[type][i] % 2 - minX;
        int y = BLOCKS[type][i] / 2 - minY;

        RectangleShape block({ 23.f, 23.f });

        block.setPosition({
            690.f + x * 28.f,
            160.f + y * 28.f
            });

        block.setFillColor(pieceColors[color]);
        block.setOutlineColor(Color(65, 95, 75));
        block.setOutlineThickness(1.f);

        window.draw(block);
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // Create game window
    RenderWindow window(
        VideoMode({ (unsigned int)WIN_W, (unsigned int)WIN_H }),
        "TETRIS",
        Style::Titlebar | Style::Close
    );

    window.setFramerateLimit(60);

    // Load font
    Font font;

    if (!font.openFromFile("PressStart2P.ttf"))
    {
        return 1;
    }

    // Game variables
    int colorNum = 1;

    bool rotate = false;
    bool gameOver = false;

    int delta_x = 0;

    float timer = 0.f;
    float delay = 0.3f;

    Clock clock;

    // Initialize game
    auto initGame = [&]()
        {
            clearBoard();

            g_score = 0;
            g_level = 1;
            g_lines = 0;
            g_paused = false;

            timer = 0.f;
            delay = 0.3f;

            gameOver = false;
            rotate = false;
            delta_x = 0;

            int type = rand() % 7;

            colorNum = PIECE_COLOR[type];

            spawnPiece(type, colorNum);

            g_nextType = rand() % 7;
            g_nextColor = PIECE_COLOR[g_nextType];
        };

    initGame();

    // Main game loop
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        // Event handling
        while (const auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
            }

            if (const auto* key =
                event->getIf<Event::KeyPressed>())
            {
                if (!gameOver && !g_paused)
                {
                    switch (key->code)
                    {
                    case Keyboard::Key::Left:
                        delta_x = -1;
                        break;

                    case Keyboard::Key::Right:
                        delta_x = 1;
                        break;

                    case Keyboard::Key::Up:
                        rotate = true;
                        break;

                    case Keyboard::Key::Space:
                        hardDrop(colorNum, gameOver);
                        timer = 0.f;
                        break;

                    default:
                        break;
                    }
                }

                if (key->code == Keyboard::Key::P && !gameOver)
                {
                    g_paused = !g_paused;
                }

                if (key->code == Keyboard::Key::R && gameOver)
                {
                    initGame();
                }

                if (key->code == Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }

        // Update game
        if (!g_paused && !gameOver)
        {
            float dropDelay = delay;

            if (Keyboard::isKeyPressed(Keyboard::Key::Down))
            {
                dropDelay = 0.05f;
            }

            rotation(rotate);
            movement(delta_x);

            timer += dt;

            fallingPiece(timer, dropDelay, colorNum, gameOver);

            delay = 0.30f - (g_level - 1) * 0.025f;

            if (delay < 0.08f)
            {
                delay = 0.08f;
            }
        }

        // Background
        window.clear(Color(225, 240, 228));

        // Main heading
        drawGameText(window, font, "TETRIS",
            22, Color(50, 95, 65), 365.f, 12.f);

        // Left information panels
        drawPanel(window, 35.f, 100.f, 225.f, 100.f);
        drawPanel(window, 35.f, 220.f, 225.f, 100.f);
        drawPanel(window, 35.f, 340.f, 225.f, 100.f);

        drawGameText(window, font, "SCORE",
            10, Color(65, 110, 75), 55.f, 120.f);

        drawGameText(window, font, std::to_string(g_score),
            15, Color(45, 85, 60), 55.f, 155.f);

        drawGameText(window, font, "LEVEL",
            10, Color(65, 110, 75), 55.f, 240.f);

        drawGameText(window, font, std::to_string(g_level),
            15, Color(45, 85, 60), 55.f, 275.f);

        drawGameText(window, font, "LINES",
            10, Color(65, 110, 75), 55.f, 360.f);

        drawGameText(window, font, std::to_string(g_lines),
            15, Color(45, 85, 60), 55.f, 395.f);

        // Game board
        RectangleShape board(
            Vector2f((float)BOARD_W, (float)BOARD_H)
        );

        board.setPosition({
            (float)BOARD_X,
            (float)BOARD_Y
            });

        board.setFillColor(Color(190, 215, 195));
        board.setOutlineColor(Color(65, 110, 75));
        board.setOutlineThickness(3.f);

        window.draw(board);

        // Draw board cells
        for (int r = 0; r < M; r++)
        {
            for (int c = 0; c < N; c++)
            {
                RectangleShape cell({
                    (float)CELL - 1.f,
                    (float)CELL - 1.f
                    });

                cell.setPosition({
                    (float)(BOARD_X + c * CELL),
                    (float)(BOARD_Y + r * CELL)
                    });

                if (gameGrid[r][c] != -1)
                {
                    int index = gameGrid[r][c];

                    if (index >= 1 && index <= 7)
                    {
                        cell.setFillColor(pieceColors[index]);
                    }
                }
                else
                {
                    cell.setFillColor(Color(210, 230, 213));
                }

                window.draw(cell);
            }
        }

        // Draw current falling piece
        for (int i = 0; i < 4; i++)
        {
            int x = point_1[i][0];
            int y = point_1[i][1];

            if (y < 0 || x < 0 || x >= N || y >= M)
            {
                continue;
            }

            RectangleShape block({
                (float)CELL - 1.f,
                (float)CELL - 1.f
                });

            block.setPosition({
                (float)(BOARD_X + x * CELL),
                (float)(BOARD_Y + y * CELL)
                });

            block.setFillColor(pieceColors[colorNum]);
            block.setOutlineColor(Color(65, 95, 75));
            block.setOutlineThickness(1.f);

            window.draw(block);
        }

        // Right information panels
        drawPanel(window, 640.f, 100.f, 225.f, 200.f);
        drawPanel(window, 640.f, 325.f, 225.f, 115.f);
        drawPanel(window, 640.f, 465.f, 225.f, 130.f);

        // Next piece
        drawGameText(window, font, "NEXT PIECE",
            9, Color(65, 110, 75), 665.f, 115.f);

        drawNext(window, g_nextType, g_nextColor);

        // Speed information
        drawGameText(window, font, "SPEED",
            9, Color(65, 110, 75), 665.f, 345.f);

        float speed = 0.30f / delay;

        drawGameText(window, font,
            std::to_string(speed).substr(0, 4) + "x",
            13, Color(45, 85, 60), 665.f, 385.f);

        // Controls
        drawGameText(window, font, "CONTROLS",
            9, Color(65, 110, 75), 665.f, 480.f);

        drawGameText(window, font, "LEFT / RIGHT",
            7, Color(65, 110, 75), 665.f, 515.f);

        drawGameText(window, font, "UP = ROTATE",
            7, Color(65, 110, 75), 665.f, 535.f);

        drawGameText(window, font, "DOWN = FAST",
            7, Color(65, 110, 75), 665.f, 555.f);

        drawGameText(window, font, "SPACE = DROP",
            7, Color(65, 110, 75), 665.f, 575.f);

        // Pause overlay
        if (g_paused)
        {
            RectangleShape overlay({
                (float)BOARD_W,
                (float)BOARD_H
                });

            overlay.setPosition({
                (float)BOARD_X,
                (float)BOARD_Y
                });

            overlay.setFillColor(Color(40, 70, 50, 190));

            window.draw(overlay);

            drawGameText(window, font, "PAUSED",
                17, Color::White,
                BOARD_X + 65.f,
                BOARD_Y + 270.f);
        }

        // Game over overlay
        if (gameOver)
        {
            RectangleShape overlay({
                (float)BOARD_W,
                (float)BOARD_H
                });

            overlay.setPosition({
                (float)BOARD_X,
                (float)BOARD_Y
                });

            overlay.setFillColor(Color(40, 70, 50, 210));

            window.draw(overlay);

            drawGameText(window, font, "GAME OVER",
                14, Color::White,
                BOARD_X + 48.f,
                BOARD_Y + 250.f);

            drawGameText(window, font, "PRESS R TO RESTART",
                7, Color::White,
                BOARD_X + 38.f,
                BOARD_Y + 300.f);
        }

        window.display();
    }

    return 0;
}
