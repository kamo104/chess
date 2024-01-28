

enum ServerMessageType {
  LIST_OF_GAMES_INFO,
  GAME_JOINED,
  GAME_NOT_JOINED, // doesn't send anything else
  PLAYER_JOINED_YOUR_GAME, // sends player_info
  PLAYER_LEFT_YOUR_GAME, // doesn't send anything else
  GAME_STARTED, // doesn't send anything else
  GAME_ENDED, // sends color of the winner
  GAME_CREATED, // sends game_id and player_color
  GAME_NOT_CREATED, // doesn't send anything else
  ENEMY_PIECE_MOVE, // sends 4 bytes: x, y, x2, y2
  PIECE_MOVE_CONFIRM, // doesn't send anything else
  PIECE_MOVE_REJECT, // doesn't send anything else
  PLAYER_ID, // sends player_id
}

enum ClientMessageType {
  GET_LIST_OF_GAMES, // doesn't send anything else ; response: LIST_OF_GAMES_INFO
  SET_PLAYER_NAME, // sends player_name - max 32 bytes ; response: PLAYER_ID
  JOIN_GAME, // sends game_id ; response: GAME_JOINED or GAME_NOT_JOINED
  CREATE_NEW_GAME, // doesn't send anything else ; response: GAME_CREATED or GAME_NOT_CREATED
  MOVE_PIECE, // sends 4 bytes: x, y, x2, y2 ; response: PIECE_MOVE_CONFIRM or PIECE_MOVE_REJECT
  LEAVE_GAME // doesn't send anything else
}