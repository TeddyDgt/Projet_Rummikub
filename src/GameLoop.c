#include "GameLoop.h"

#include "Draw.h"
#include "Players.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void distribute_initial_tiles(Deck *d, Player players[], int num_players) {
  for (int p = 0; p < num_players; p++) {
    for (int i = 0; i < HAND_SIZE_START; i++) {
      Tile t = draw_tile(d);
      add_tile_to_player(&players[p], t);
    }
    sort_player_hand(&players[p], 1);
  }
}

int tile_value_for_start(Tile t) {
  if (t.is_joker)
    return 0;
  return t.value;
}

int determine_first_player(Deck *d, int num_players) {
  Tile drawn[MAX_PLAYERS];
  int best_idx = 0;
  int best_val = -1;

  for (int i = 0; i < num_players && i < MAX_PLAYERS; i++) {
    drawn[i] = draw_tile(d);
    int v = tile_value_for_start(drawn[i]);
    if (v > best_val) {
      best_val = v;
      best_idx = i;
    }
  }

  return best_idx;
}

int is_game_over(Player players[], int num_players, Deck *d) {
  if (!players || num_players <= 0)
    return 1;

  for (int i = 0; i < num_players; i++) {
    if (players[i].hand_count == 0)
      return 1;
  }

  // Minimal condition: deck empty => game ends (you can enhance later:
  // "blocked" logic)
  if (d && d->top <= 0)
    return 1;

  return 0;
}

void calculate_final_scores(Player players[], int num_players) {
  if (!players || num_players <= 0)
    return;

  int winner = -1;
  for (int i = 0; i < num_players; i++) {
    if (players[i].hand_count == 0) {
      winner = i;
      break;
    }
  }
  if (winner < 0)
    winner = 0;

  int pot = 0;
  for (int i = 0; i < num_players; i++) {
    if (i == winner)
      continue;
    int pen = calculate_hand_penalty(&players[i]);
    players[i].score -= pen;
    pot += pen;
  }
  players[winner].score += pot;
}

void save_scores_to_file(Player players[], int num_players, char *filename) {
  if (!players || num_players <= 0 || !filename)
    return;

  FILE *f = fopen(filename, "a");
  if (!f) {
    perror("fopen");
    return;
  }

  fprintf(f, "--- Scores Rummikub ---\n");
  for (int i = 0; i < num_players; i++) {
    fprintf(f, "%s;%d\n", players[i].name, players[i].score);
  }
  fprintf(f, "\n");
  fclose(f);
}

Tile find_tile(Player *p, int id) {
    for (int i = 0; i < p->hand_count; i++) {
        if (p->hand[i].id == id) return p->hand[i];
    }
    Tile err = {-1, 0, 0, 0}; 
    return err;
}

Combinaison* parse_player_move_terminal(Player* p, char *input, int *out_count) {
    Combinaison *liste = NULL;
    int nb_combs = 0;

    char *saveptr_grp, *saveptr_id;
    char *groupe_str = strtok_r(input, " ", &saveptr_grp);

    while (groupe_str != NULL) {
        Combinaison comb_temp;
        comb_temp.tiles = NULL;
        comb_temp.count = 0;
        comb_temp.type = 0; 

        char *id_str = strtok_r(groupe_str, ",", &saveptr_id);
        while (id_str != NULL) {
            int id = atoi(id_str);
            Tile t = find_tile(p, id);

            if (t.id != -1) {
                comb_temp.count++;
                comb_temp.tiles = realloc(comb_temp.tiles, sizeof(Tile) * comb_temp.count);
                comb_temp.tiles[comb_temp.count - 1] = t;
            }
            id_str = strtok_r(NULL, ",", &saveptr_id);
        }

        if (comb_temp.count > 0) {
            nb_combs++;
            liste = realloc(liste, sizeof(Combinaison) * nb_combs);
            liste[nb_combs - 1] = comb_temp;
        }

        groupe_str = strtok_r(NULL, " ", &saveptr_grp);
    }

    *out_count = nb_combs;
    
    return liste;
}