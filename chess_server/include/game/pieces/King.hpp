#pragma once

#include "Piece.hpp"

class King : public Piece{
    public:
        // King(std::weak_ptr<Board> board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        King(Board* board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        virtual bool is_checking_king() override;
        virtual bool can_move(Move move) override;
};
