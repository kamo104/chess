#pragma once

#include "Piece.hpp"
#include <game/Move.hpp>

class Rook : public Piece{
    public:
        // Rook(std::weak_ptr<Board> board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        Rook(Board* board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        virtual bool is_checking_king() override;
        virtual bool can_move(Move move) override;
};
