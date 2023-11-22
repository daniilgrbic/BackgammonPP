# Name: Init Singleplayer Match

### Short description
- Initialize game state for choosen game variant
- Initialize backgammon bot
  
### Actors
- Player

### Pre-Conditions
- Game options are selected

### Post-Conditions 
- Game has started

### Main Flow

```
1. Player clicks 'Play SinglePlayer' button
2. Player is prompted with game options, proceed to 'Choose Game Options' use case, when finished, proceed to step (3)
3. Player clicks 'Start Game' button
4. Load weights and links for neural network from config file
5. Create neural network from weights and links
6. Set current game options as default for next game
7. Proceed to 'Play Game' use case
```

### Alternative Flows
- None

### Subflows
- None

### Additional info
- None

