#include "networking.h"

void clientLogic(int server_socket){
  char input[WORD_SIZE];
  char buff[WORD_SIZE];
  printf("enter a command: ");
  fgets(input, WORD_SIZE, stdin);
  input[strcspn(input, "\n")] = 0;

  // command logic
  // check which command
  if (strcmp(input, "help") == 0) {
    printf("To send a message, type 'chat'\n");
    printf("To make a letter guess, type 'guess'\n");
    printf("To make a word guess, type 'guess-word'\n");
    printf("To exit the game, type 'quit'\n");
  }
  else if (strcmp(input, "quit") == 0) {
    write(server_socket, "quit", 5);
    exit(0);
  }
  else if (strcmp(input, "guess") == 0) {
    write(server_socket, "guess", 6);
    usleep(50);
    read(server_socket, buff, WORD_SIZE);
    if (strcmp(buff, "no") == 0) {
        printf("Wait for your turn!\n");
    }
    else if (strcmp(buff, "yes") == 0) {
        printf("guess a letter: ");
        fgets(input, WORD_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;
        write(server_socket, input, WORD_SIZE);
    }
  }
  else {
    printf("Invalid command. Type 'help' for a list of commands.\n");
  }
  
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
