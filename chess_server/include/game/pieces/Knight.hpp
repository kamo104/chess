#pragma once

#include "Piece.hpp"

class Knight : public Piece{
    public:
        // Knight(std::weak_ptr<Board> board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        Knight(Board* board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        virtual bool is_checking_king() override;
        virtual bool can_move(Move move) override;
};
