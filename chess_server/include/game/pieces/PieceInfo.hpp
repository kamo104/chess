#pragma once

enum PieceType : char{
    EMPTY = 0,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

enum PieceColor : char{
    BLACK = 0,
    WHITE,
    NONE
};

struct PiecePosition{
    PiecePosition(char x, char y) : x(x), y(y){ }
    char x;
    char y;
};


struct piece_info{
    piece_info(PieceType type, PieceColor color, PiecePosition position) : type(type), color(color), position(position){}
    PieceType type;
    PieceColor color;
    PiecePosition position;
};