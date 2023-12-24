# Name: Play Local Game 

### Short description
- Play a game versus another human opponent on the same computer
  
### Actors
- Player 1
- Player 2

### Pre-Conditions
- Lobby is created
- Player 1 selected to play local game
- Game is started

### Post-Conditions 
- Player is offered to save the game log.

### Main Flow

```
1. Determine which player goes first by a dice roll.
    1.1 If player 1 goes first, skip to (2)
    1.2 If player 2 goes first, skip to (6)
2. Determine if doubling possible
3. Player 1 decides whether to use the doubling cube    
    3.1. If doubling, request a move from player 2
        3.1.1. If player 2 accepts, double the stakes, and player 2 takes ownership of the doubling cube  
        3.1.2. If player 2 rejects, jump to (12)  
4. Follow steps (5) - (8) from the Play Game use case
5. Check if player 1 satisfies winning condition  
    5.1. If they do, skip to (12)  
6. Determine if doubling possible
7. Player 2 decides whether to use the doubling cube.
    7.1 If doubling, request a move from player 1
        7.1.1 If player 1 accepts, double the stakes, and player 1 takes ownership of the doubling cube
        7.1.2 If player 1 rejects, skip to (12)
8. Follow steps (5) - (6) from the Play Game use case
9. Player 2 plays a valid turn
10. Check if player 2 satisfies winning condition
    11.1 If he does, skip to (12)
11. Return to (2)
12. Determine the winner of the game  
    12.1. If player 1 won, declare player 1 the winner  
    12.2. If player 2 won, declare player 2 the winner
13. Follow steps (14) - (17) from the Play Game use case

```

### Alternative Flows
```
A1: Player exits the application. If ingame, discard current game. End of path.
```

### Subflows
- None

### Additional info
- None

