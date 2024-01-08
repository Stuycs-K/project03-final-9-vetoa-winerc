#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

void err(int line);
char checkLetterGuess(char letter);
char checkWordGuess(char* target);
void guessResult(int result);
struct game_info* startGame(struct game_info* game);
struct game_info* advanceGame(struct game_info* game);
