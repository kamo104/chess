#include <algorithm>

#include "game/pieces/PieceInfo.hpp"
#include "game/pieces/Piece.hpp"
#include "game/pieces/Queen.hpp"

#include "game/Board.hpp"

#include "game/Move.hpp"

bool Queen::is_checking_king(){
    Piece* king = _board->get_enemy_king(info.color);

    int king_x = king->info.position.x;
    int king_y = king->info.position.y;
    int x = info.position.x;
    int y = info.position.y;

    return can_move(Move(x,y,king_x,king_y,PieceType::EMPTY));
}

bool Queen::can_move(Move move){
    int x = move.x;
    int y = move.y;
    int x2 = move.x2;
    int y2 = move.y2;

    if((abs(x2-x) != abs(y2-y)) && x2 != x && y2 != y)
        return false;

    if(x2 == x && y2 > y){
        for(int i=y+1;i<y2;i++){
            if(_board->get_piece(x,i)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 == x && y2 < y){
        for(int i=y-1;i>y2;i--){
            if(_board->get_piece(x,i)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 > x && y2 == y){
        for(int i=x+1;i<x2;i++){
            if(_board->get_piece(i,y)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 < x && y2 == y){
        for(int i=x-1;i>x2;i--){
            if(_board->get_piece(i,y)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 > x && y2 > y){
        for(int i=1;i<x2-x;i++){
            if(_board->get_piece(x+i,y+i)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 > x && y2 < y){
        for(int i=1;i<x2-x;i++){
            if(_board->get_piece(x+i,y-i)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 < x && y2 > y){
        for(int i=1;i<x-x2;i++){
            if(_board->get_piece(x-i,y+i)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }
    if(x2 < x && y2 < y){
        for(int i=1;i<x-x2;i++){
            if(_board->get_piece(x-i,y-i)->info.type != PieceType::EMPTY)
                return false;
        }
        return true;
    }

    return false;
}

