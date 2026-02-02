#include "Draw.h"
#include "Players.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Distribue les tuiles de depart aux joueurs.
 *
 * @param d Parameter d.
 * @param players Parameter players.
 * @param num_players Parameter num_players.
 * @return None.
 */
void distribute_initial_tiles(Deck *d, Player players[], int num_players) {
  for (int p = 0; p < num_players; p++) {
    for (int i = 0; i < HAND_SIZE_START; i++) {
      add_tile_to_player(&players[p], draw_tile(d));
    }
    // Utilisation de ta signature avec le flag 1 (par couleur)
    sort_player_hand(&players[p], 1);
  }
}

/**
 * @brief Determine le premier joueur a partir de la pioche.
 *
 * @param d Parameter d.
 * @param num_players Parameter num_players.
 * @return Result value.
 */
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

/**
 * @brief Verifie si game over.
 *
 * @param players Parameter players.
 * @param num_players Parameter num_players.
 * @param d Parameter d.
 * @return Result value.
 */
int is_game_over(Player players[], int num_players, Deck *d) {
  for (int i = 0; i < num_players; i++) {
    if (players[i].hand_count == 0)
      return 1;
  }
  return (d->top <= 0);
}

/**
 * @brief Calcule final scores.
 *
 * @param players Parameter players.
 * @param num_players Parameter num_players.
 * @return None.
 */
void calculate_final_scores(Player players[], int num_players) {
  if (!players || num_players <= 0) {
    return;
  }

  int penalties[4] = {0};
  int winner = -1;

  for (int i = 0; i < num_players; i++) {
    penalties[i] = calculate_hand_penalty(&players[i]);
    if (players[i].hand_count == 0 && winner == -1) {
      winner = i;
    }
  }

  if (winner == -1) {
    int best_penalty = penalties[0];
    winner = 0;
    for (int i = 1; i < num_players; i++) {
      if (penalties[i] < best_penalty) {
        best_penalty = penalties[i];
        winner = i;
      }
    }
  }

  if (players[winner].hand_count == 0) {
    int total = 0;
    for (int i = 0; i < num_players; i++) {
      if (i == winner) {
        continue;
      }
      players[i].score = -penalties[i];
      total += penalties[i];
    }
    players[winner].score = total;
  } else {
    int total = 0;
    int winner_penalty = penalties[winner];
    for (int i = 0; i < num_players; i++) {
      if (i == winner) {
        continue;
      }
      int delta = penalties[i] - winner_penalty;
      players[i].score = -delta;
      total += delta;
    }
    players[winner].score = total;
  }

  printf("\n--- TABLEAU DES SCORES ---\n");
  for (int i = 0; i < num_players; i++) {
    printf("%s : %d points\n", players[i].name, players[i].score);
  }
}

/**
 * @brief Realise l'operation trim_newline.
 *
 * @param s Parameter s.
 * @return None.
 */
static void trim_newline(char *s) {
  if (!s) {
    return;
  }
  size_t len = strlen(s);
  while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
    s[len - 1] = '\0';
    len--;
  }
}

/**
 * @brief Realise l'operation trim_spaces.
 *
 * @param s Parameter s.
 * @return None.
 */
static void trim_spaces(char *s) {
  if (!s) {
    return;
  }
  char *start = s;
  while (*start == ' ' || *start == '\t') {
    start++;
  }
  if (start != s) {
    memmove(s, start, strlen(start) + 1);
  }
  size_t len = strlen(s);
  while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t')) {
    s[len - 1] = '\0';
    len--;
  }
}

/**
 * @brief Realise l'operation find_score_entry.
 *
 * @param param Parameter param.
 * @param count Parameter count.
 * @param name Parameter name.
 * @return Result value.
 */
static int find_score_entry(char names[][64], int count, const char *name) {
  for (int i = 0; i < count; i++) {
    if (strcmp(names[i], name) == 0) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Sauvegarde scores to file.
 *
 * @param players Parameter players.
 * @param num_players Parameter num_players.
 * @param filename Parameter filename.
 * @return None.
 */
void save_scores_to_file(Player players[], int num_players, char *filename) {
  if (!players || num_players <= 0 || !filename) {
    return;
  }

  char names[128][64];
  int totals[128];
  int count = 0;

  FILE *in = fopen(filename, "r");
  if (in) {
    char line[128];
    while (fgets(line, sizeof(line), in)) {
      trim_newline(line);
      if (line[0] == '\0' || line[0] == '-') {
        continue;
      }
      char *sep = strchr(line, ';');
      if (!sep) {
        continue;
      }
      *sep = '\0';
      char *name = line;
      char *score_str = sep + 1;
      trim_spaces(name);
      trim_spaces(score_str);
      if (name[0] == '\0') {
        continue;
      }
      int score = (int)strtol(score_str, NULL, 10);
      int idx = find_score_entry(names, count, name);
      if (idx >= 0) {
        totals[idx] += score;
      } else if (count < 128) {
        strncpy(names[count], name, sizeof(names[count]) - 1);
        names[count][sizeof(names[count]) - 1] = '\0';
        totals[count] = score;
        count++;
      }
    }
    fclose(in);
  }

  for (int i = 0; i < num_players; i++) {
    const char *pname = players[i].name ? players[i].name : "Joueur";
    int idx = find_score_entry(names, count, pname);
    if (idx >= 0) {
      totals[idx] += players[i].score;
    } else if (count < 128) {
      strncpy(names[count], pname, sizeof(names[count]) - 1);
      names[count][sizeof(names[count]) - 1] = '\0';
      totals[count] = players[i].score;
      count++;
    }
  }

  FILE *out = fopen(filename, "w");
  if (!out) {
    return;
  }

  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  char time_buf[64] = {0};
  if (tm_info) {
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", tm_info);
  }

  fprintf(out, "--- Cumul %s ---\n", time_buf[0] ? time_buf : "n/a");
  for (int i = 0; i < count; i++) {
    fprintf(out, "%s;%d\n", names[i], totals[i]);
  }
  fclose(out);
}





