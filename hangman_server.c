#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#define MAX_CLIENTS 8
#define WORD_SIZE 128
#define MESSAGE_SIZE 512

void error(int number, char* message) {
    if (number == -1) {
        printf("%s\n", message);
        printf("%s\n", strerror(errno));
        exit(1);
    }
}

/*
    creates the server socket for clients to connect to
*/
int server_setup() {
    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    char* PORT = "1738";

    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    error(getaddrinfo(NULL, PORT , hints, &results), "getaddrinfo failed");  //NULL is localhost or 127.0.0.1

    //create socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);\

    //this code allows the port to be freed after program exit.  (otherwise wait a few minutes)
    int yes = 1;
    error(( setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ), "setsockopt failed");

    error(bind(listen_socket, results->ai_addr, results->ai_addrlen), "bind failed");
    listen(listen_socket, 3);//3 clients can wait to be processed
    printf("Listening on port %s\n",PORT);

    free(hints);
    freeaddrinfo(results);
    return listen_socket;
}

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

    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

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
                int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
                printf("Connected to new client\n");

                // add the new socket to the list of socket connections
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_sockets[i] != -1) {
                        client_sockets[i] = client_socket;
                        current_clients++;
                    }
                }
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