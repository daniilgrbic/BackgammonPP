# Name: Play Single Player Game

### Short description
- Play a game versus a bot
  
### Actors
- Player
- Bot

### Pre-Conditions
- Lobby is created
- Player selected to play versus a Bot
- Game is started.

### Post-Conditions 
- Player is offered to save the game log.

### Main Flow

```
1. Initialize the bot from a config file.
2. Determine if the player or the bot goes first by a dice roll.
    2.2 If the player goes first, skip to (3)
    2.3 If the bot goes first, skip to (7)
3. Determine if doubling possible
4. Player decides whether to use the doubling cube    
    4.1. If doubling, request a move from the bot
        4.1.1. If the bot accepts, double the stakes, and the bot takes ownership of the doubling cube  
        4.1.2. If the bot rejects, jump to (13)  
5. Follow steps (5) - (8) from the Play Game use case
6. Check if player satisfies winning condition  
    9.1. If they do, skip to (13)  
7. Determine if doubling possible
8. The bot decides whether to use the doubling cube.
    8.1 If the bot offers a double, prompt the player to either accept it or reject it.
        8.1.1 If the player accepts, double the stakes, and the bot takes ownership
        8.1.2 If the player rejects, skip to (13)
9. Follow steps (5) - (6) from the Play Game use case
10. The bot plays a valid turn
11. Check if the bot satisfies winning condition
    11.1 If it does, skip to (13)
12. Return to (3)
13. Determine the winner of the game  
    13.1. If player won, declare player the winner  
    13.2. If the bot won, declare bot the winner
14. Follow steps (14) - (17) from the Play Game use case


```

### Alternative Flows
```
A1: Player exits the application. If ingame, discard current game. End of path.
```
### Subflows
- None

### Additional info
- None
