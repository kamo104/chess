import 'package:flutter/material.dart';

import 'app_state.dart';
import 'login_page.dart';
import 'game_page.dart';

class HomePage extends StatefulWidget {
  const HomePage(this.state,{super.key, required this.title});

  final AppState state;
  final String title;

  @override
  State<HomePage> createState() => _HomePageState();
}

bool oneTime = true;

class _HomePageState extends State<HomePage> {
  Null onGameNotify(){
    setState((){});
  }

  Null onGameJoinAccepted(){
    if(widget.state.gameJoinRequest.accepted.value == 1){
      widget.state.gameJoinRequest.accepted.value = -1;
      Navigator.push(
        context,
        MaterialPageRoute(builder: (context) => GamePage(widget.state, title: widget.title)),
      );
    }
  }

  Null onGameCreateAccepted(){
    if(widget.state.gameCreateRequest.accepted.value == true){
      widget.state.gameCreateRequest.accepted.value = false;
      Navigator.push(
        context,
        MaterialPageRoute(builder: (context) => GamePage(widget.state, title: widget.title)),
      );
    }
  }

  @override
  void didUpdateWidget(HomePage page){
    super.didUpdateWidget(page);
    widget.state.gamesNotifier.removeListener(onGameNotify);
    widget.state.gameJoinRequest.accepted.removeListener(onGameJoinAccepted);
    widget.state.gameCreateRequest.accepted.removeListener(onGameCreateAccepted);
    
    widget.state.gamesNotifier.addListener(onGameNotify);
    widget.state.gameJoinRequest.accepted.addListener(onGameJoinAccepted);
    widget.state.gameCreateRequest.accepted.addListener(onGameCreateAccepted);
  }

  @override
  void dispose() {
    super.dispose();
    widget.state.gamesNotifier.removeListener(onGameNotify);
    widget.state.gameJoinRequest.accepted.removeListener(onGameJoinAccepted);
    widget.state.gameCreateRequest.accepted.removeListener(onGameCreateAccepted);
  }

  @override
  void initState() {
    super.initState();
    widget.state.gamesNotifier.addListener(onGameNotify);
    widget.state.gameJoinRequest.accepted.addListener(onGameJoinAccepted);
    widget.state.gameCreateRequest.accepted.addListener(onGameCreateAccepted);
  }

  @override
  Widget build(BuildContext context) {

    

    WidgetsBinding.instance.addPostFrameCallback((_) {
      if(widget.state.username.value == '' && oneTime == true){ {
        oneTime = false;
        Navigator.push(
          context,
          MaterialPageRoute(builder: (context) => LoginPage(widget.state)),
        );
      }
    }});

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
              Expanded(
                child: ListView.builder(
                  itemCount: widget.state.gamesNotifier.games.length,
                  itemBuilder: (context, index) {
                    return ListTile(
                      title: Text("${widget.state.gamesNotifier.games[index].owner.name} vs ${widget.state.gamesNotifier.games[index].opponent.name}"),
                      onTap: (){
                        widget.state.currentGame = widget.state.gamesNotifier.games[index];
                        widget.state.gameJoinRequest.sendToServer.value = true;
                      }
                    );
                  },
                ),
              ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: (){
          widget.state.currentGame = null;
          widget.state.sendGameCreateRequest();
        },
        
        tooltip: 'New Game',
        child: const Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
