#pragma once

#include "game/Move.hpp"
#include "game/pieces/PieceInfo.hpp"

#include "pieces/Piece.hpp"
#include "pieces/Pawn.hpp"
#include "pieces/Rook.hpp"
#include "pieces/Knight.hpp"
#include "pieces/Bishop.hpp"
#include "pieces/Queen.hpp"
#include "pieces/King.hpp"


enum GameStatus : char{
    ONGOING = 0,
    WHITE_CHECKED,
    BLACK_CHECKED,
    ILLEGAL_MOVE,
    WHITE_WON,
    BLACK_WON,
    DRAW
};

class Board {

    Piece* _board[8][8];
    // std::shared_ptr<Board> self;
    Board* self;


    bool is_legal_move(PieceColor player_color, Move move);
    GameStatus check_after_move(bool player_color, Move move);

    GameStatus check_check(PieceColor color);

    public:

        Board();
        ~Board();

        Piece* get_piece(char x, char y) const;
        GameStatus move(PieceColor player_color, Move move);
        bool is_enemy(char x, char y, Piece* piece) const;

        Piece* get_king(PieceColor color) const;
        Piece* get_enemy_king(PieceColor color) const;

        // void init(std::shared_ptr<Board> board);
        void init(Board* board);
};