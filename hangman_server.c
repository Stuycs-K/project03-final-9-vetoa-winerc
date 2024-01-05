#include "networking.h"

/*
    Arguments: the file descriptor of the socket connection that the command is coming from
    Behavior: Handles the command (if it is a command) or produces an error message
    Returns: none
*/
void handle_command(int client_socket) {

}

int main(){
    // opens the socket for clients to connect to
    int listen_socket = server_setup();
    fd_set read_fds;

    // initializes arrays of usernames and of sockets to clients
    int current_clients = 0;
    int client_sockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1;
    }
    char usernames[8][WORD_SIZE];

    char buff[1025]="";

    while(1){

        FD_ZERO(&read_fds);
        int max_socket = listen_socket;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] != -1) {
                FD_SET(client_sockets[i], &read_fds);
                if (client_sockets[i] > max_socket) {
                    max_socket = client_sockets[i];
                }
            }
        }
        FD_SET(listen_socket,&read_fds);
        int i = select(max_socket+1, &read_fds, NULL, NULL, NULL);

        // if LISTEN socket -- connect the client to the server
        if (FD_ISSET(listen_socket, &read_fds)) {
            if (current_clients == 8) {
                printf("Server full: no more users allowed\n");
            }
            else {
                //accept the connection
                int client_socket = server_tcp_handshake(listen_socket);
                printf("Connected to new client\n");

                // add the new socket to the list of socket connections
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_sockets[i] == -1) {
                        client_sockets[i] = client_socket;
                        current_clients++;
                        break;
                    }
                }
                printf("total clients connected: %d\n", current_clients);
            }
        }

        //if client socket, handle the command read from the client
        else {
            for(int i = 0; i < MAX_CLIENTS; i++) {
                if (FD_ISSET(client_sockets[i], &read_fds)) {
                    handle_command(client_sockets[i]);
                }
            }
        }
    }
    return 0;
}