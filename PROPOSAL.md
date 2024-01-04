# Final Project Proposal

## Group Members:

Charlie Winer, Adrian Veto
       
# Intentions:
We are creating a multiplayer (2-8 players) hangman game. Players take turns trying to guess a word of variable length. There will be a chat for players to communicate potential guesses with one another. Varying difficulty levels based on word length (ie. more guesses for a 12 letter word than a 5 letter word)
    
# Intended usage:

Users connect to a server. The server assigns a random guessing order and word chooser. There will also be an option for the server to choose a random word based on a dictionary. 
After the game has started, players take turns guessing letter, OR, they have the opportunity to attempt to guess the entire word. After each guess, all connected users get sent the updated status of the game. Throughout the game users can chat with each other. 

# Technical Details:

- Server opens and creates a socket to allow users to connect. All users connect to the same server at once. Server can support up to 8 players at once. Server keeps a list of all connected users stored.
- At the start of the game, the server randomizes the guess order of all users and either prompts the guesser for a word or gets a random dictionary word using WordNet.
- Server prompts one user at a time for a guess of the letter or word and checks it against the true word. True word and hangman status are stored in a .txt file which is sent to all users in human-readable format after each game.
- The whole time, a chat server is open that users can access using a CLI command to communicate with each other over the server. No DMs, just whole group chat.
    
# Intended pacing:

- By Jan 5: Working server with user connections from up to 8 machines.
- By Jan 8: Implement simple blackjack on a single machine without using pipes/sockets. Just word checking and guess giving, etc.
- By Jan 10: Implement blackjack with connections from devices on the server.
- Jan 10-11: Add random order and words from dictionary. One partner starts working on chat server.
- Jan 12: Finish all user commands for guesses, chats, etc.
- By Jan 16: Chat server finished, all commands linked to their functions. Sighandling. Bugfixing and testing on lab machines. Work on presentation.
