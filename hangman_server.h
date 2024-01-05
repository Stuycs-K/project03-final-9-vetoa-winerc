#ifndef GAME_INFO
#define GAME_INFO
struct game_info {
    int num_clients;
    int* client_sockets;
    char** usernames;
    char gamemode;
};
#endif

#ifndef HANGMAN_SERVER_H
#define HANGMAN_SERVER_H
void client_command(int client_socket, struct game_info* game);
void server_command(struct game_info* game);

#endif