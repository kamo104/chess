#pragma once

#include <game/pieces/PieceInfo.hpp>

struct user_info{
  unsigned int id=0;
  char name[32];
};

struct player_info
{
  player_info(){};
  player_info(user_info user, PieceColor color) : user(user), color(color){};
  user_info user;
  PieceColor color;
};