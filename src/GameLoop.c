#include "Draw.h"
#include "Players.h"
#include "structs.h"
#include <stdio.h>

void distribute_initial_tiles(Deck *d, Player players[], int num_players) {
  for (int p = 0; p < num_players; p++) {
    for (int i = 0; i < HAND_SIZE_START; i++) {
      add_tile_to_player(&players[p], draw_tile(d));
    }
    // Utilisation de ta signature avec le flag 1 (par couleur)
    sort_player_hand(&players[p], 1);
  }
}

int determine_first_player(Deck *d, int num_players) {
  int best_idx = 0;
  int max_val = -1;
  for (int i = 0; i < num_players; i++) {
    Tile t = draw_tile(d);
    if (t.value > max_val) {
      max_val = t.value;
      best_idx = i;
    }
  }
  return best_idx;
}

int is_game_over(Player players[], int num_players, Deck *d) {
  for (int i = 0; i < num_players; i++) {
    if (players[i].hand_count == 0)
      return 1;
  }
  return (d->top <= 0);
}

void calculate_final_scores(Player players[], int num_players) {
  printf("\n--- TABLEAU DES SCORES ---\n");
  for (int i = 0; i < num_players; i++) {
    int penalty = 0;
    for (int j = 0; j < players[i].hand_count; j++) {
      penalty += (players[i].hand[j].is_joker) ? 30 : players[i].hand[j].value;
    }
    players[i].score -= penalty;
    printf("%s : %d points\n", players[i].name, players[i].score);
  }
}