#include "networking.h"

/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  struct addrinfo *hints, *results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  err(getaddrinfo(server_address, "3333", hints, &results), "getaddrinfo failed");
  
  //create the socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  
  //connect to the server
  err(connect(sd, results->ai_addr, results->ai_addrlen), "connect to server failed");
  
  free(hints);
  freeaddrinfo(results);

  return sd;
}

/*Accept a connection from a client
 *return the to_client socket descriptor
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    //accept the client connection
    client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
  
    return client_socket;
}

/*Create and bind a socket.
* Place the socket in a listening state.
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo *hints, *results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  err(getaddrinfo(NULL, "3333", hints, &results), "getaddrinfo failed");
  
  //create the socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");
  
  //bind the socket to address and port
  err(bind(sd, results->ai_addr, results->ai_addrlen), "server_setup: bind failed");
  
  //set socket to listen state
  listen(sd, 10);

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);
  
  return sd;
}

void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}
