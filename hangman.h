#ifndef HANGMAN_H
#define HANGMAN_H
struct game_info* checkLetterGuess(struct game_info* game, char letter);
struct game_info* checkWordGuess(struct game_info* game, char* target);
struct game_info* startGame(struct game_info* game);
void guessResult(int result);
struct game_info* startGame(struct game_info* game);
#endif