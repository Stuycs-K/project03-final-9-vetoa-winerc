[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/SQs7pKlr)
# Hangman

### Purple Giraffe

Adrian Veto, Charlie Winer
       
### Project Description:

This is a multiplayer hangman game played using terminals. It supports 1-8 players, all connected to a server using sockets. Users have the option to choose a word or have the computer choose a word for them. Guessing order is random and will progress as the game advances.
  
### Instructions:
**Compile the program**:
```make compile```

**To run the server**:
```make server```

**To make a client**:
```
make client
make client ARGS=IP
./client.out IP
```
IP is the IP address of the computer hosting the server

#### How does the user interact with this program?
##### Server commands: 
*Start game*:
```
server command: start
```
*Change gamemode*:
```
server command: gamemode
New gamemode ('computer' or 'user'): computer
```
*Change number of guesses*:
```
server command: num_guesses
Enter the number of guesses you want: 4
```
*Change the word chooser*:
```
server command: chooser
Enter the index of the new chooser: 1
```
*Get game info*:
```
server command: status
```
*List of possible commands*
```
server command: help
```
*Exit the game*
```
server command: quit
```
##### Client commands: 
*Send a chat message*:
```
enter a command: chat
```
*View the game status*:
```
enter a command: status
```
*Make a letter guess*:
```
enter a command: guess
```
*Make a word guess*:
```
enter a command: guess-word
```
*List of possible commands*:
```
enter a command: help
```
*Disconnect from the game*:
```
enter a command: quit
```
