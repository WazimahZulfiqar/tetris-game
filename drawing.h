#pragma once

#include "utils.h"
#include "pieces.h"
#include <string>

//DRAWING FUNCTIONS

void drawPolkaBg(sf::RenderWindow& win);

void drawCard(sf::RenderWindow& win,
    float x, float y, float w, float h);

void drawButton(sf::RenderWindow& win, sf::Font& font,
    const std::string& label,
    float x, float y, float w, float h);

void drawTextC(sf::RenderWindow& win, sf::Font& font,
    const std::string& str, unsigned size,
    sf::Color col, float cx, float y);

void drawTextL(sf::RenderWindow& win, sf::Font& font,
    const std::string& str, unsigned size,
    sf::Color col, float x, float y);

bool mouseIn(sf::Vector2i m,
    float x, float y, float w, float h);

void drawBoardFrame(sf::RenderWindow& win);

void drawGhost(sf::RenderWindow& win, sf::Sprite& sprite);

void drawNextPiece(sf::RenderWindow& win, sf::Sprite& sprite,
    float boxX, float boxY,
    float boxW, float boxH);