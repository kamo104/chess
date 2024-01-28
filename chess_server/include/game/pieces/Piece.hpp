#pragma once

#include <game/pieces/PieceInfo.hpp>
#include <game/Move.hpp>

#include <memory>

class Board;


class Piece {
    protected:
        // std::weak_ptr<Board> _board;
        Board* _board;

    public:
        piece_info info;

        bool is_enemy(char x, char y){return false;};
        virtual bool is_checking_king(){return false;};
        virtual bool can_move(Move move){return false;};
        Piece(Board* board, piece_info pieceInfo) : _board(board), info(pieceInfo){}
        // Piece(std::weak_ptr<Board> board, piece_info pieceInfo) : _board(board), info(pieceInfo){}

        virtual ~Piece() {}
};