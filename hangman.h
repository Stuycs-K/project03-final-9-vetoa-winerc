#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#ifndef HANGMAN_H
#define HANGMAN_H
void err(int line);
struct game_info* checkLetterGuess(struct game_info* game, char letter);
struct game_info* checkWordGuess(struct game_info* game, char* target);
void guessResult(int result);
struct game_info* startGame(struct game_info* game);
struct game_info* advanceGame(struct game_info* game);
#endif
