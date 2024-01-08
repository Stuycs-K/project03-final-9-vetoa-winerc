#include "hangman_server.h"
#include "hangman.h"
#include "networking.h"

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
    checkLetterGuess(guess);
}

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
    checkWordGuess(buff);
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
    printf("-----------------------------------------------\n");
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
        printf("To change the word chooser, type 'chooser'\n");
        printf("To get current game info, type 'status'\n");
        printf("To end the game, type 'quit'\n");
    }
    else if (strcmp(command, "start") == 0) {
        startGame(game);
    }
    else if (strcmp(command, "gamemode") == 0) {
        game = change_gamemode(game);
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
                printf("Connected to new client\n");

                // add the new socket to the list of socket connections
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (game->client_sockets[i] == -1) {
                        game->client_sockets[i] = client_socket;
                        game->num_clients++;
                        break;
                    }
                }
                printf("total clients connected: %d\n", game->num_clients);
            }
        }

        // if input to the server, handle commands for the server
        else if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            // printf("server_command called\n");
            server_command(game);
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