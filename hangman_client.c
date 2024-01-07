#include "networking.h"

void clientLogic(int server_socket){
  char input[WORD_SIZE];
  printf("enter a command: ");
  fgets(input, WORD_SIZE, stdin);
  
  error(write(server_socket, input, WORD_SIZE), "client: writing to server failed");
  // printf("written to client\n");
  usleep(50);
  error(read(server_socket, input, WORD_SIZE), "client: reading from server failed");
  printf("string from server: %s", input);
}

int main(int argc, char** argv) {
    char* IP = "127.0.0.1";
    if(argc>1){
        IP=argv[1];
    }
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    while (1) {
        clientLogic(server_socket);
        usleep(50);
    }
}
