# Dev Log:

## Adrian

### 2024-01-03
In class: Started PROPOSAL.md, including the overview of what will be done. Brainstorming features. 40min.\
At home: Finished PROPOSAL.md, which included the timeline of work to do and the breakup of work between group members. Also researched dictionaries to use for the computer word generator. 25min.

### 2024-01-04
In class: worked on understanding how multiple clients connect to a server and select command. Implemented demo code. 40min.\
At home: Implemented networking procedures for handshakes. Server is now functional and accepts multiple connections from pipes, storing them in a list of file descriptors. Clients connect to the server and then finish for now. Server is functional but needs processing for commands sent from clients. 35min.

### 2024-01-05
In class: Added struct for game info that can be used for executing user and server commands. Started to integrate user and server commands from the terminal and implement server behavior.

### 2024-01-06
At home: Finished all server command implementations: starting the game, changing gamemode, changing chooser, printing game status, and exiting, along with a help message for confused users. Implemented the initial client structure and tested to make sure that clients could connect to the server successfully. Client-server infrastructure is up and running, ready to implement hangman once the game is ready. Created a new struct for the game info so as not to pass multiple arguments every time. Planned client behavior for tomorrow. 90min.

### 2024-01-08
In class: support for guessing words from the client. Started brainstorming chat server. Helped Charlie with the implementation of the struct game_info* functions in hangman.h. 40min\
At home: Added support for client commands 'help', 'quit', and 'guess'. Guess is waiting on the hangman game implementation but the server-client interaction works. Have to discuss how guessing and game setup will work in hangman. 25min.

### 2024-01-09
In class: Client_status command for client receiving server status. Incorporated startGame and set word from Charlie's branch to start testing my functions. BUG: server can't exit before client, ctrl+C leads to weird messages: work on quit and handling for SIGQUIT.
At home: Formatting for server command prompts and buffer flushing.

## Charlie


### 2024-01-03 
In class: Came up with idea for hangman game. 40min.\
At home: Added template for readme. Worked on proposal. 15min.

### 2024-01-04 
In class: Worked on basic hangman functions for either making a letter guess or making a word guess. Tried (and failed) to implement random word functionality. 40min. \
At home: Finished the word guess and letter guess check methods (using fopen instead). Fixed method so it is case insensitive. Guess methods and guess display methods are functional (will switch off printf for display once combined w/ server. 40min. 

### 2024-01-05
In class: Handling for guess progress. Guesses are stored in a separate buffer that is updated each time a user makes a letter guess and written into the second line of word.txt. Unknown letters are displayed as asterisks. 40min.

### 2024-01-07
At Home: Updated guess progress handling (cumulative for both letter and word guesses), fopen uses r+ now instead of r and w. Added guess amount functionality for checkletterguess (rewrites 3rd line of file for each letter guess). Need to discuss how checkwordguess will work for incrementing the guess amount tomorrow. 30 min.

### 2024-01-08
In Class: Used the struct gameinfo to start and advance games within hangman. Started planning how gameflow will work with server. 40min.\
At Home: Added support for starting the game. Revised so that start game and advance game match. 30min.


