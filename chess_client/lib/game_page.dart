import 'package:chess_client/game.dart';
import 'package:flutter/material.dart';

import 'app_state.dart';


class GamePage extends StatefulWidget {
  const GamePage(this.state,{super.key, required this.title});

  final AppState state;
  final String title;

  @override
  State<GamePage> createState() => _GamePageState();
}

class _GamePageState extends State<GamePage> {

  Null onOpponentMove(){
    if(widget.state.opponentMove.newMove.value == true){
      widget.state.opponentMove.newMove.value = false;
      widget.state.currentGame!.board[widget.state.opponentMove.x2][widget.state.opponentMove.y2] = widget.state.currentGame!.board[widget.state.opponentMove.x][widget.state.opponentMove.y];
      widget.state.currentGame!.board[widget.state.opponentMove.x][widget.state.opponentMove.y] = PieceType.EMPTY;
      setState(() {});
    }
  }

  Null onPieceMove(){
    if(widget.state.gameMoveRequest.accepted.value == 1){
      widget.state.gameMoveRequest.accepted.value = -1;
      widget.state.currentGame!.board[widget.state.gameMoveRequest.x2][widget.state.gameMoveRequest.y2] = widget.state.currentGame!.board[widget.state.gameMoveRequest.x][widget.state.gameMoveRequest.y];
      widget.state.currentGame!.board[widget.state.gameMoveRequest.x][widget.state.gameMoveRequest.y] = PieceType.EMPTY;
      setState(() {});
    }
  }

  Null onOpponentJoin(){
    if(widget.state.opponentJoined.value == true){
      widget.state.opponentJoined.value = false;
      setState(() {});
    }
  }
  Null onGameEnd(){
    if(widget.state.gameEnd.value == true){
      widget.state.gameEnd.value = false;
      setState(() {});
    }
  }

  Null onOpponentLeft(){
    if(widget.state.opponentLeft.value == true){
      widget.state.opponentLeft.value = false;
      bool myColor = widget.state.currentGame!.owner.id == widget.state.myId ? widget.state.currentGame!.owner.color : widget.state.currentGame!.opponent.color;
      widget.state.currentGame!.status = myColor ? GameStatus.WHITE_WON : GameStatus.BLACK_WON;
      setState(() {});
    }
  }
  Null onLeaveRequest(){
    if(widget.state.leaveRequest.value == true){
      widget.state.leaveRequest.value = false;
    }
  }

  @override
  void didUpdateWidget(GamePage page){
    super.didUpdateWidget(page);
    widget.state.opponentMove.newMove.removeListener(onOpponentMove);
    widget.state.gameMoveRequest.accepted.removeListener(onPieceMove);
    widget.state.opponentJoined.removeListener(onOpponentJoin);
    widget.state.gameEnd.removeListener(onGameEnd);
    widget.state.opponentLeft.removeListener(onOpponentLeft);
    widget.state.leaveRequest.removeListener(onLeaveRequest);

    widget.state.opponentMove.newMove.addListener(onOpponentMove);
    widget.state.gameMoveRequest.accepted.addListener(onPieceMove);
    widget.state.opponentJoined.addListener(onOpponentJoin);
    widget.state.gameEnd.addListener(onGameEnd);
    widget.state.opponentLeft.addListener(onOpponentLeft);
    widget.state.leaveRequest.addListener(onLeaveRequest);
  }

  @override
  void dispose() {
    super.dispose();
    widget.state.opponentMove.newMove.removeListener(onOpponentMove);
    widget.state.gameMoveRequest.accepted.removeListener(onPieceMove);
    widget.state.opponentJoined.removeListener(onOpponentJoin);
    widget.state.gameEnd.removeListener(onGameEnd);
    widget.state.opponentLeft.removeListener(onOpponentLeft);
    widget.state.leaveRequest.removeListener(onLeaveRequest);
  }

  @override
  void initState(){
    super.initState();
    widget.state.opponentMove.newMove.addListener(onOpponentMove);
    widget.state.gameMoveRequest.accepted.addListener(onPieceMove);
    widget.state.opponentJoined.addListener(onOpponentJoin);
    widget.state.gameEnd.addListener(onGameEnd);
    widget.state.opponentLeft.addListener(onOpponentLeft);
    widget.state.leaveRequest.addListener(onLeaveRequest);
  }


  @override
  Widget build(BuildContext context) {
    const colorBlack = Color.fromARGB(120, 206, 107, 26);
    const colorWhite = Color.fromARGB(255, 207, 183, 151);


    List<Widget> gameBoard = [];
    bool myColor = widget.state.currentGame!.owner.id == widget.state.myId ? widget.state.currentGame!.owner.color : widget.state.currentGame!.opponent.color;
    int start = myColor ? 7 : 0;
    int end = myColor ? -1 : 8;
    
    for(var i=start; myColor ? i>end : i<end; myColor ? i-- : i++){
      for(var j=start; myColor ? j>end : j<end; myColor ? j-- : j++){
        onAccept(DragTargetDetails<List<int>> data) {
          widget.state.gameMoveRequest.x = data.data[0];
          widget.state.gameMoveRequest.y = data.data[1];
          widget.state.gameMoveRequest.x2 = i;
          widget.state.gameMoveRequest.y2 = j;
          widget.state.gameMoveRequest.sendToServer.value = true;
        };
        containerAdd(Color color){
          gameBoard.add(Container(
            color: color,
            child: DragTarget(
              builder: (
                BuildContext context,
                List<dynamic> accepted,
                List<dynamic> rejected,
              ) {
                return Draggable(
                  feedback: pieceImages[widget.state.currentGame!.board[i][j]]!,
                  data: List<int>.from([i,j]),
                  childWhenDragging: Container(
                    color: color
                  ),
                  child: pieceImages[widget.state.currentGame!.board[i][j]]!
                );
              },
              onAcceptWithDetails: onAccept,
            ),
          ));
        }
        
        if((i+j)%2==0){
          containerAdd(colorBlack);
        }
        else{
          containerAdd(colorWhite);
        }
      }
    }

    var boardDisplay = <Widget>[
      Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          SizedBox(
            width: 60*8,
            height: 60*8,
            child: GridView.count(
              crossAxisCount: 8,
              children: gameBoard
            )
          )
        ],
      ),
      const Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          SizedBox(
            width: 60,
          )
        ],
      ),
      Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          if(widget.state.currentGame!.status == GameStatus.WHITE_WON)
          Text("${widget.state.currentGame!.opponent.color == true ? widget.state.currentGame!.opponent.name : widget.state.currentGame!.owner.name} (White) won",textScaler: TextScaler.linear(2)),
          if(widget.state.currentGame!.status == GameStatus.BLACK_WON)
          Text("${widget.state.currentGame!.opponent.color == false ? widget.state.currentGame!.opponent.name : widget.state.currentGame!.owner.name} (Black) won",textScaler: TextScaler.linear(2)),
          Text('${widget.state.currentGame!.owner.name} as ${widget.state.currentGame!.owner.color == true ? "white" : "black"}'),
          const Text(' vs '),
          Text('${widget.state.currentGame!.opponent.name} as ${widget.state.currentGame!.opponent.color == true ? "white" : "black"}'),
        ],
      )
    ];
    
    return PopScope(
      onPopInvoked: (bool didPop){
        widget.state.leaveRequest.value = true;
      },
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
          title: Text(widget.title),
        ),
        body: Center(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: boardDisplay,
            ),
        )
      )
    );
  }
}
