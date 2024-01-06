#include "hangman_server.h"
#include "hangman.h"
#include "networking.h"

/*
    Arguments: the file descriptor of the socket connection that the command is coming from
    Behavior: Handles the command (if it is a command) or produces an error message
    Returns: none
*/
void client_command(int client_socket, struct game_info* game) {

}

/*
    Arguments: none
    Behavior: handles commands from the server given through stdin
    Returns: none
*/
void server_command(struct game_info* game) {
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
    else if (strcmp(command, "")) {

    }
    else {
        printf("Invalid command. For instructions, type 'help'\n");
    }
}

int main(){
    // opens the socket for clients to connect to
    int listen_socket = server_setup();
    fd_set read_fds;
    struct game_info* game = malloc(sizeof(struct game_info*));
    // computer word chooser
    game->gamemode = 0;
    game->num_clients = 0;
    game->client_sockets = malloc(sizeof(int) * MAX_CLIENTS);
    game->usernames = malloc(sizeof(char*) * MAX_CLIENTS);
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
                    client_command(game->client_sockets[i], game);
                }
            }
        }
    }
    return 0;
}