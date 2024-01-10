#include "hangman_server.h"
#include "hangman.h"
#include "networking.h"

/* 
    Arguments: the game information, the index of the client
    Behavior: checks if the character is included in the word
    Returns: none
*/
void client_guess(int index, struct game_info* game) {
    char buff[WORD_SIZE];
    // if the user isn't the guesser
    if (index != game->guesser) {
        write(game->client_sockets[index], "no", 3);
        return;
    }
    write(game->client_sockets[index], "yes", 4);
    usleep(50);
    read(game->client_sockets[index], buff, WORD_SIZE);
    char guess = buff[0];
    game = checkLetterGuess(game, guess);
}

/* 
    Arguments: the game information, the index of the client
    Behavior: checks if the word is correct
    Returns: none
*/
void client_guess_word(int index, struct game_info* game) {
    char buff[WORD_SIZE];
    // if the user isn't the guesser
    if (index != game->guesser) {
        write(game->client_sockets[index], "no", 3);
        return;
    }
    write(game->client_sockets[index], "yes", 4);
    usleep(50);
    read(game->client_sockets[index], buff, WORD_SIZE);
    game = checkWordGuess(game, buff);
}

/* 
    Arguments: the index for the client socket, the game info struct
    Behavior: gets the game status, writes it to a string, and sends the string to the client
    Returns: none
*/
void client_status(int index, struct game_info* game) {
    char* buff = malloc(MESSAGE_SIZE);
    if (index == game->guessing_order[game->guesser_index]) {
        sprintf(buff, "\nThe word is %s.\nThe incorrect guesses are %s\nThere are %d guesses remaining.\nIt's your turn to guess!\n", game->current_word, game->failed_guesses, game->num_guesses);
    }
    else if (index == game->chooser) {
        sprintf(buff, "\nThe word is %s.\nThe incorrect guesses are %s\nThere are %d guesses remaining.\nYou're the word chooser!\n", game->current_word, game->failed_guesses, game->num_guesses);
    }
    else {
        sprintf(buff, "\nThe word is %s.\nThe incorrect guesses are %s\nThere are %d guesses remaining.\nIt's not your turn to guess.\n", game->current_word, game->failed_guesses, game->num_guesses);
    }
    write(game->client_sockets[index], buff, MESSAGE_SIZE);
    free(buff);
}

/*
    Arguments: the index of the socket with the command, a struct with the game info
    Behavior: Handles the command (if it is a command) or produces an error message
    Returns: none
*/
void client_command(int index, struct game_info* game) {
    // printf("recieved command from client\n");
    char buff[WORD_SIZE];
    read(game->client_sockets[index], buff, WORD_SIZE);
    if (strcmp(buff, "quit") == 0) {
        game->client_sockets[index] = -1;
        game->usernames[index] = "";
        game->num_clients--;
        printf("Client disconnected. %d clients connected.\n", game->num_clients);
    }
    else if (strcmp(buff, "status") == 0) {
        client_status(index, game);
    }
    else if (strcmp(buff, "guess") == 0) {
        client_guess(index, game);
    }
    else if (strcmp(buff, "guess-word") == 0) {
        client_guess_word(index, game);
    }
}

/*
    Arguments: the struct containing the game info of the current game
    Behavior: prompts the user for input, then changes the gamemode accordingly
    Returns: the struct containing the game info after changing gamemode
*/
struct game_info* change_gamemode(struct game_info* game) {
    // printf("old gamemode: %d\n", game->gamemode);
    printf("New gamemode ('computer' or 'user'): ");
    char buff[20];
    fgets(buff, 19, stdin);
    buff[strcspn(buff, "\n")] = 0;
    if (strcmp(buff, "computer") == 0) {
        game->gamemode = COMPUTER_CHOOSING;
    }
    else if (strcmp(buff, "user") == 0) {
        game->gamemode = USER_CHOOSING;
    }
    else {
        printf("invalid gamemode, changing gamemode failed\n");
    }
    // printf("new gamemode: %d\n", game->gamemode);
    return game;
}

/*
    Arguments: the current game info struct
    Behavior: Prompts the user for input for a new chooser, then changes the game info
    Returns: The new game info struct
*/
struct game_info* change_chooser(struct game_info* game) {
    // printf("old chooser: %d\n", game->chooser);
    printf("Enter the index of the new chooser: ");
    char buff[3];
    fgets(buff, 3, stdin);
    buff[strcspn(buff, "\n")] = 0;
    int newChooser = atoi(buff);
    if (newChooser < game->num_clients && newChooser >= 0) {
        game->chooser = newChooser;
    }
    else {
        printf("new chooser doesn't exist, command failed\n");
    }
    // printf("new chooser: %d\n", game->chooser);
    return game;
}

/*
    Arguments: the current game info struct
    Behavior: prints the information from the struct in human-readable format
    Returns: none
*/
void print_status(struct game_info* game) {
    printf("--------------Current Game Status--------------\n");
    if (game->gamemode == COMPUTER_CHOOSING) {
        printf("Gamemode: computer choosing\n");
    }
    else {
        printf("Gamemode: user choosing\n");
    }
    if (game->guessing_order != NULL) {
        printf("Guessing order:\n");
        for (int i = 0; i < game->num_clients; i++) {
            printf("[%d]: %s", i, game->usernames[game->guessing_order[i]]);
            if (game->guessing_order[i] == game->chooser && game->gamemode == USER_CHOOSING) {
                printf(" (chooser)\n");
            }
            else if (game->guessing_order[i] == game->guesser) {
                printf(" (guesser)\n");
            }
            else {
                printf("\n");
            }
        }
    }
    else {
        for (int i = 0; i < game->num_clients; i++) {
            printf("[%d]: %s", i, game->usernames[i]);
            if (i == game->chooser && game->gamemode == USER_CHOOSING) {
                printf(" (chooser)\n");
            }
            else if (i == game->guesser) {
                printf(" (guesser)\n");
            }
            else {
                printf("\n");
            }
        }
    }
    printf("Correct word: %s\n", game->real_word);
    printf("Current user word: %s\n", game->current_word);
    printf("Num guesses remaining: %d\n", game->num_guesses);
    printf("-----------------------------------------------\n");
}

/*
    Arguments: the current game info struct
    Behavior: prompts the server for the number of guesses they want the game to have, changes the number of guesses
    Returns: the updated game info struct
*/
struct game_info* change_num_guesses(struct game_info* game) {
    printf("Enter the number of guesses you want: ");
    char buff[4];
    fgets(buff, 4, stdin);
    buff[strcspn(buff, "\n")] = 0;
    int newGuesses = atoi(buff);

    game->num_guesses = newGuesses;

    return game;
}

/*
    Arguments: the struct with the current game info
    Behavior: handles commands from the server given through stdin
    Returns: the struct with the new game info
*/
struct game_info* server_command(struct game_info* game) {
    char command[COMMAND_SIZE] = "";
    read(fileno(stdin), command, COMMAND_SIZE);
    command[strcspn(command, "\n")] = 0;

    //handling commands
    if (strcmp(command, "help") == 0) {
        printf("To start the game, type 'start'\n");
        printf("To change gamemode, type 'gamemode'\n");
        printf("To change the number of guesses, type 'num_guesses'\n");
        printf("To change the word chooser, type 'chooser'\n");
        printf("To get current game info, type 'status'\n");
        printf("To end the game, type 'quit'\n");
    }
    else if (strcmp(command, "start") == 0) {
        startGame(game);
        printf("Game started\n");
    }
    else if (strcmp(command, "gamemode") == 0) {
        game = change_gamemode(game);
    }
    else if (strcmp(command, "num_guesses") == 0) {
        game = change_num_guesses(game);
    }
    else if (strcmp(command, "chooser") == 0) {
        game = change_chooser(game);
    }
    else if (strcmp(command, "status") == 0) {
        print_status(game);
    }
    else if (strcmp(command, "quit") == 0) {
        exit(0);
    }
    else {
        printf("Invalid command. For instructions, type 'help'.\n");
    }
    return game;
}

int main(){
    // opens the socket for clients to connect to
    int listen_socket = server_setup();
    printf("server command: ");
    fflush(stdout);
    fd_set read_fds;
    struct game_info* game = malloc(sizeof(struct game_info*));
    // computer word chooser
    game->gamemode = COMPUTER_CHOOSING;
    game->num_clients = 0;
    game->client_sockets = malloc(sizeof(int) * MAX_CLIENTS);
    game->usernames = malloc(sizeof(char*) * MAX_CLIENTS);
    game->guesser = 0;
    game->guessing_order = NULL;
    // first user to join is automatically the chooser
    game->chooser = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        game->usernames[i] = malloc(WORD_SIZE);
    }
    // printf("all memory allocated\n");
    
    // initializes arrays of usernames and of sockets to clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        game->client_sockets[i] = -1;
    }

    while(1){
        // add every socket to the select statement
        FD_ZERO(&read_fds);
        int max_socket = listen_socket;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (game->client_sockets[i] != -1) {
                FD_SET(game->client_sockets[i], &read_fds);
                if (game->client_sockets[i] > max_socket) {
                    max_socket = game->client_sockets[i];
                }
            }
        }
        FD_SET(listen_socket,&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        int i = select(max_socket+1, &read_fds, NULL, NULL, NULL);

        // if LISTEN socket -- connect the client to the server
        if (FD_ISSET(listen_socket, &read_fds)) {
            if (game->num_clients == 8) {
                printf("Server full: no more users allowed\n");
            }
            else {
                //accept the connection
                int client_socket = server_tcp_handshake(listen_socket);
                printf("\nConnected to new client\n");

                // add the new socket to the list of socket connections
                game->client_sockets[game->num_clients] = client_socket;

                // get client username
                read(client_socket, game->usernames[game->num_clients], WORD_SIZE);
                // printf("username is %s\n", game->usernames[game->num_clients]);

                game->num_clients++;
                printf("total clients connected: %d\n", game->num_clients);
                printf("server command: ");
                fflush(stdout);
            }
        }

        // if input to the server, handle commands for the server
        else if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            // printf("server_command called\n");
            server_command(game);
            printf("server command: ");
            fflush(stdout);
        }

        //if client socket, handle the command read from the client
        else {
            for(int i = 0; i < MAX_CLIENTS; i++) {
                if (FD_ISSET(game->client_sockets[i], &read_fds)) {
                    client_command(i, game);
                }
            }
        }
    }
    return 0;
}