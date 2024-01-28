#pragma once

#include "Piece.hpp"

class Queen : public Piece{
    public:
        // Queen(std::weak_ptr<Board> board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        Queen(Board* board, piece_info pieceInfo) : Piece(board,pieceInfo){}
        virtual bool is_checking_king() override;
        virtual bool can_move(Move move) override;
};
