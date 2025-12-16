#include <stdio.h>
#include <stdlib.h>

#include "game_logic.h"

void free_players(Player players[], int n) {
  for (int i = 0; i < n; i++) {
    free(players[i].hand);
    players[i].hand = NULL;
    players[i].hand_count = 0;
  }
}

int main(void) {
  Deck deck;
  Table table;
  Player players[2];
  int num_players = 2;

  init_deck(&deck);
  init_table(&table);

  init_player(&players[0], "Alice");
  init_player(&players[1], "Bob");

  int first = determine_first_player(&deck, num_players);
  printf("Joueur qui commence: %s\n\n", players[first].name);

  distribute_initial_tiles(&deck, players, num_players);
  for (int i = 0; i < num_players; i++) {
    print_player_hand(&players[i]);
    printf("\n");
  }

  // Mini boucle de jeu console: chaque joueur pioche 1 tuile par tour.
  // (Le vrai placement sur la table viendra ensuite.)
  int current = first;
  int turn = 1;
  while (!is_game_over(players, num_players, &deck) && turn <= 20) {
    printf("--- Tour %d: %s ---\n", turn, players[current].name);
    if (deck.top > 0) {
      Tile t = draw_tile(&deck);
      add_tile_to_player(&players[current], t);
      sort_player_hand(&players[current], 1);
      printf("%s pioche une tuile (id=%d).\n", players[current].name, t.id);
    } else {
      printf("Pioche vide.\n");
    }
    printf("Tuiles restantes dans la pioche: %d\n\n", deck.top);

    current = (current + 1) % num_players;
    turn++;
  }

  calculate_final_scores(players, num_players);
  printf("=== Scores finaux ===\n");
  for (int i = 0; i < num_players; i++) {
    printf("%s : %d\n", players[i].name, players[i].score);
  }
  save_scores_to_file(players, num_players, "scores.txt");

  free_table(&table);
  free_players(players, num_players);
  return 0;
}