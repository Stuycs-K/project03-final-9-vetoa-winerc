#include "networking.h"

/*
  Arguments: the socket to the server
  Behavior: handles user input from the terminal, and then passes the commands along to the server
  Returns: none
*/
void clientInput(int server_socket){
  char input[WORD_SIZE];
  char buff[WORD_SIZE];
  // printf("enter a command: ");
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
  // quit
  else if (strcmp(input, "quit") == 0) {
    write(server_socket, "quit", 5);
    exit(0);
  }
  // guess
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
  // guess-word
  else if (strcmp(input, "guess-word") == 0) {
    write(server_socket, "guess-word", 6);
    usleep(50);
    read(server_socket, buff, WORD_SIZE);
    if (strcmp(buff, "no") == 0) {
        printf("Wait for your turn!\n");
    }
    else if (strcmp(buff, "yes") == 0) {
        printf("guess a word: ");
        fgets(input, WORD_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;
        write(server_socket, input, WORD_SIZE);
    }
  }
  else if (strcmp(input, "chat") == 0) {
    // chat server implementation -- implement later
    printf("chat command\n");
  }
  else {
    printf("Invalid command. Type 'help' for a list of commands.\n");
  }
  
}

void displayServerMessage(int server_socket) {
  char buff[MESSAGE_SIZE];
  printf("\nFrom Server:%s\n", buff);
} 

int main(int argc, char** argv) {
    char* IP = "127.0.0.1";
    if(argc>1){
        IP=argv[1];
    }
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);
    while (1) {
      printf("enter a command: ");
      fflush(stdout);
      int i = select(server_socket + 1, &read_fds, NULL, NULL, NULL);
      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        clientInput(server_socket);
      }
      else if (FD_ISSET(server_socket, &read_fds)) {
        displayServerMessage(server_socket);
      }
      usleep(50);
    }
}
