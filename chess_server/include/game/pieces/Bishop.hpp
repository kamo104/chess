#pragma once

#include "Piece.hpp"

class Bishop : public Piece{
    public:
        // Bishop(std::weak_ptr<Board> board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        Bishop(Board* board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        virtual bool is_checking_king() override;
        virtual bool can_move(Move move) override;
};
