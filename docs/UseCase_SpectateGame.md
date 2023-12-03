# Name: Spectate Game

### Short description
- Spectate a joined match
- Chat with other spectators

### Actors
- Spectator

### Pre-Conditions
- The lobby has been created.
- The user joined the lobby before the match started.
- Other players have been chosen as match participants.
- The match has started.

### Post-Conditions 
- The user is offered to save the game log.

### Main Flow
```
1. The app window displays the spectated game and the chat with other spectators.
2. While the match is still going on, the user can choose to do one of the following:
    1.1 Go backwards/forwards one move in current game.
    1.2 Send message to other spectators, optionally choosing a receiver.
        1.2.1 If the user has chosen a specific receiver, only they will receive the message.
        1.2.2 Otherwise, all other spectators will receive the message.
```

### Alternative Flows
```
A1: User exits the application. End of path.
```

```
A2: Host exits the application after user connected. Let the user know and return to main menu. End of path.
```

### Subflows
- None

### Additional info
- None
