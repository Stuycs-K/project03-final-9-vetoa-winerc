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
struct game_info* checkLetterGuess(struct game_info* game, char letter);
struct game_info* checkWordGuess(struct game_info* game, char* target);
void guessResult(int result);
void startGame(struct game_info* game);
#endif