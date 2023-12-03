# Name: Play Network Game

### Short description
- Play a game versus another human opponent on the different computer

### Actors
- Our Player
- Hosting Player

### Pre-Conditions
- Game options are selected
- Our player joins the lobby, waiting for hosted game
- Match between them is found

### Post-Conditions
- Game replay (log) is saved

### Main Flow
```
1. Player color is determined  
    1.1 If white, skip to (2)  
    1.2 If black, skip to (10)  
2. Display the board   
3. Determine if doubling possible  
4. Player decides whether to use the doubling cube    
    4.1. If doubling, prompt the opponent    
        4.1.1. If opponent accepts, double the stakes, and opponent takes ownership of the doubling cube  
        4.1.2. If opponent rejects, jump to (13)  
5. Roll the dice    
6. Determine valid moves    
7. Player plays their turn  
    7.1. Player chooses a valid move  
    7.2. Player decides, whether to end turn or undo the chosen move  
        7.2.1. If turn ended, proceed to step (8)  
        7.2.2. If turn undone, return to step (7)  
8. Update the game state and board  
9. Check if player satisfies winning condition  
    9.1. If they do, skip to (13)  
10. Wait for opponent to make a move  
    10.1. If opponent offers doubling the stakes  
        10.1.1. If player accepts, double the stakes, and player takes ownership of the doubling cube  
        10.1.2. If player rejects, jump to (13)  
11. Check if opponent satisfies winning condition  
    11.1. If they do, skip to (13)  
12. Return to step (3)  
13. Determine the winner of the game  
    13.1. If player won, declare player the winner  
    13.2. If opponent won, declare opponent the winner  
14. Proceed to the "Save Game Replay" use case, when finished, proceed to step (15)  
15. Display the post-game screen  
16. Prompt the player to rematch  
    16.1. If player accepts, prompt the opponent to rematch  
        16.1.1. If opponent is another player, wait for them to answer the prompt  
            16.1.1.1. If opponent accepts, jump to step (1)  
            16.1.1.2. If opponent rejects, jump to step (17)  
        16.1.2. If opponent is computer, jump to step (1)  
    16.2. If player rejects, jump to step (17)   
17. Return to main menu  
```

### Alternative Flows
```
A1: Player can't find hosted game. Wait and after some time, return to the main menu. End of path.
```
```
A2: Player exits the application. If ingame, discard current game. Tell the player that the opponent disconnected and that he won the game, discard the game and return to main menu. End of path.
```

### Subflows
- None

### Additional info
```
During the game, each move is saved to persistent storage.
```
