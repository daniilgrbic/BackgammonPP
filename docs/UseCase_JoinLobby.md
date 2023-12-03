# Name: Join Lobby

### Short description
- User enters nickname and host IP address.
- User connects to host. 
- Depending on if the host choose this user, they can either Play Network Game or Spectate Game. 

### Actors
- Remote Player
- Spectator

### Pre-Conditions
- Host creates lobby.
- Host selected to play network game.

### Post-Conditions
- User is either spectator or remote player.

### Main Flow
```
1. User selects Join Lobby in Main Menu.
2. User inputs username and host's IP address.
3. Check if IP address is valid
    3.1 If not, let the user know and return to step (2)
    3.2 Otherwise, proceed to step (4)
4. Check if Username is unique
    4.1 If not, let the user know and return to step (2)
    4.2 Otherwise, proceed to step (5)
5. Wait for host to choose opponent
    5.1 If host selected the user, proceed to "Play Network Game" use case
    5.2 Otherwise, proceed to "Spectate Game" use case
```

### Alternative Flows
```
A1: Player exits the application before host chooses opponent. Remove player's name from host's opponent options. End of path.
```

```
A2: Host exits the application after user connected. Let the user know and return to main menu. End of path.
```

### Subflows
- None

### Additional info
```
Before game, player must be made aware of IP address of the host through other means.
```
