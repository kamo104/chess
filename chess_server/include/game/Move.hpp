#pragma once

#include "game/pieces/PieceInfo.hpp"

struct Move{
    Move(int x, int y, int x2, int y2, PieceType promotion = PieceType::EMPTY) : x(x), y(y), x2(x2), y2(y2), promotion(promotion) { }
    char x;
    char y;
    char x2;
    char y2;
    PieceType promotion;
};