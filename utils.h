
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// WINDOW & BOARD DIMENSIONS
const int WIN_W = 900;
const int WIN_H = 700;
const int N = 10;
const int M = 20;
const int CELL = 30;

const int BOARD_W = N * CELL;
const int BOARD_H = M * CELL;
const int BOARD_X = (WIN_W - BOARD_W) / 2;
const int BOARD_Y = (WIN_H - BOARD_H) / 2;

const std::string WINDOW_TITLE = "TETRIS";

enum class Screen { MENU, GAME, SETTINGS, GAMEOVER };

inline int gameGrid[M][N];
inline int point_1[4][2];
inline int point_2[4][2];

inline int g_score = 0;
inline int g_level = 1;
inline int g_lines = 0;
inline bool g_paused = false;

inline Screen g_screen = Screen::MENU;

inline int g_nextType = 0;
inline int g_nextColor = 1;



// ================================================================
//  KAWAII COLOUR PALETTE
// ================================================================
const sf::Color C_BG{ 200, 240, 215 };   // light green background
const sf::Color C_BOARD{ 210, 245, 225 };   // soft mint board
const sf::Color C_DEEP_PINK{ 255,  20, 147 };
const sf::Color C_HOT_PINK{ 255, 105, 180 };
const sf::Color C_BTN{ 100, 160, 220 };   // button fill
const sf::Color C_BTN_SHADOW{ 50, 90, 140 };
const sf::Color C_WHITE_SOFT{ 255, 245, 252 };
const sf::Color C_TEXT_PINK{ 40, 80, 130 };
const sf::Color C_LAVENDER{ 200, 168, 216 };
const sf::Color C_BORDER{ 58,  32,  64 };
const sf::Color C_GRID{ 170, 120, 210,  50 };

inline void clearBoard() {
    for (int r = 0; r < M; r++)
        for (int c = 0; c < N; c++)
            gameGrid[r][c] = -1;
}
