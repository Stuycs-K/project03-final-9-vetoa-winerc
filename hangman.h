#ifndef GAME_INFO
#define GAME_INFO
struct game_info {
    int num_clients;
    int* client_sockets;
    char** usernames;
    char gamemode;
};
#endif

#ifndef HANGMAN_H
#define HANGMAN_H
char checkLetterGuess(char letter);
char checkWordGuess(char* target);
void guessResult(int result);
void startGame(struct game_info* game);
#endif