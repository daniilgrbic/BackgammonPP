# Name: Create Lobby

### Short description
- The Local Player creates game lobby where he chooses game options
  
### Actors
- Local Player

### Pre-Conditions
- The application is launched and displays the menu

### Post-Conditions 
- The game is started according to the selected options

### Main Flow
```
1. The Local Player clicks 'Create Lobby' button from the menu.
2. The Local Player inputs his username.
3. The Local Player chooses type of his opponent:
    3.1. If the Local Player decides to play against another Local Player:
        3.1.1. Other Local Player inputs his username.
        3.1.2. If there are remote users connected:
            3.1.2.1. Remote users proceed to 'Spectate Game' use case.
        3.1.3. Proceed to 'Play Local Game' use case.
    3.2. If the Local Player decides to play against Bot:
        3.2.1. If there are remote users connected:
            3.2.1.1. Remote users proceed to 'Spectate Game' use case.
        3.2.2. Proceed to 'Play Single Player Game' use case.
    3.3. If the Local Player decides to play against Remote Player:
        3.3.1. The Local Player chooses his opponent from the list of remote users.
        3.3.2. If there are other remote users connected:
            3.3.2.1. Other remote users proceed to 'Spectate Game' use case:
        3.3.3. Proceed to 'Play Network Game' use case.
```

### Alternative Flows
```
A2: The Local Player has chosen to play against a Remote Player but the Remote Player has not yet connected. The Local Player can choose to wait or proceed to alternative flow A2. End of path.
```
```
A2: The Local Player exits the Create Lobby. If there are remote users connected notify them about cancellation. End of path.
```

### Subflows
- None

### Additional info
```
Before game, player must notify remote users about his IP address through other means.
```


