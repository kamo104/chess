#pragma once

#include "Piece.hpp"

class Pawn : public Piece{
    public:
        // Pawn(std::weak_ptr<Board> board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        Pawn(Board* board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        virtual bool is_checking_king() override;
        virtual bool can_move(Move move) override;
};
