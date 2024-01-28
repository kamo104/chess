#include <memory>

#include <server/Player.hpp>
#include <game/Game.hpp>
#include <game/Board.hpp>

Game::Game(game_info gameinfo){
    this->info = gameinfo;
    Board* board = new Board();
    this->_board = board;
    this->_board->init(board);
};

Game::Game(){
    this->info.id = 0;
    this->info.owner.user.id = 0;
    this->info.opponent.user.id = 0;
    this->info.current_turn = PieceColor::WHITE;
    // this->_board = std::make_shared<Board>();
    Board* board = new Board();
    this->_board = board;
    this->_board->init(board);
};

GameStatus Game::move(PieceColor playerColor, Move move){
    if(_board->get_piece(move.x, move.y)->info.color != playerColor){
        return GameStatus::ILLEGAL_MOVE;
    }

    GameStatus status = _board->move(info.current_turn,move);

    if(status == GameStatus::ONGOING || status == GameStatus::WHITE_CHECKED || status == GameStatus::BLACK_CHECKED)
    info.current_turn = info.current_turn == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;

    return status;
}
