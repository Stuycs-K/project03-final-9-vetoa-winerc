#ifndef GAME_INFO
#define GAME_INFO
struct game_info {
    int num_clients;
    int* client_sockets;
    char** usernames;
    char gamemode;
    int chooser;
};
#endif

#ifndef HANGMAN_SERVER_H
#define HANGMAN_SERVER_H
void client_command(int client_socket, struct game_info* game);
struct game_info* server_command(struct game_info* game);
struct game_info* change_gamemode(struct game_info* game);
struct game_info* change_chooser(struct game_info* game);
void print_status(struct game_info* game);
#endif

#define COMPUTER_CHOOSING 0
#define USER_CHOOSING 1