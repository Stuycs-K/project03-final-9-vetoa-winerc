#include "hangman_server.h"
#include "networking.h"
#include <stdlib.h>
#include <time.h>
struct game_info* checkLetterGuess(struct game_info* game, char letter) {
 return 0;
}

struct game_info* checkWordGuess(struct game_info* game, char* target) {
 return 0;
}

void guessResult(int result) {
 return;
}

struct game_info* setStartingWord(struct game_info* game) {
  if (game->gamemode == COMPUTER_CHOOSING) {
    game->real_word = "hangman"; // hardcoded, will change later
  }
  else if (game->gamemode == USER_CHOOSING) {
    game->real_word = malloc(128);
    write(game->client_sockets[game->chooser], "choose word", 12);
    usleep(50);
    read(game->client_sockets[game->chooser], game->real_word, 128);
  }
  // set current word based on real word
  game->current_word = malloc(strlen(game->real_word));
  for (int i = 0; i < strlen(game->real_word); i++) {
    game->current_word[i] = '*';
  }
  game->current_word[strlen(game->real_word)] = 0;
  return game;
}

struct game_info* startGame(struct game_info* game) {
  // guessing order
  srand(time(NULL));
  game->guessing_order = malloc(sizeof(int) * game->num_clients);
  for (int i = 0; i < game->num_clients; i++) {
    game->guessing_order[i] = -1;
  }
  for (int i = 0; i < game->num_clients; i++) {
    // finds a random index array that hasn't been assigned
    int j = rand() % game->num_clients;
    while (game->guessing_order[j] != -1) {
      j = rand() % game->num_clients;
    }
    game->guessing_order[j] = i;
  }

  game->guesser = game->guessing_order[0];
  game->guesser_index = 0;
  // printf("guessing order: ");
  // for (int i = 0; i < game->num_clients; i++) {
  //   printf(" %d ", game->guessing_order[i]);
  // }
  // printf("\n");
  // hardcoded, will change later
  game->num_guesses = 5;

  game = setStartingWord(game);
 
  return game;
}