#include "structs.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialise player.
 *
 * @param p Parameter p.
 * @param name Parameter name.
 * @return None.
 */
void init_player(Player *p, const char *name) {
  p->name = strdup(name);
  p->score = 0;
  p->hand_count = 0;
  p->is_ai = 0;
  p->has_initial_meld = 0;
  // ALLOCATION CRUCIALE : on réserve de la place pour éviter les IDs fantômes
  p->hand = malloc(sizeof(Tile) * MAX_TILES);
}

/**
 * @brief Ajoute tile to player.
 *
 * @param p Parameter p.
 * @param t Parameter t.
 * @return None.
 */
void add_tile_to_player(Player *p, Tile t) {
  if (p->hand_count < MAX_TILES) {
    p->hand[p->hand_count] = t;
    p->hand_count++;
  }
}

/**
 * @brief Retire tile from hand.
 *
 * @param p Parameter p.
 * @param tile_id Parameter tile_id.
 * @return None.
 */
void remove_tile_from_hand(Player *p, int tile_id) {
  int found_idx = -1;
  for (int i = 0; i < p->hand_count; i++) {
    if (p->hand[i].id == tile_id) {
      found_idx = i;
      break;
    }
  }
  if (found_idx != -1) {
    for (int i = found_idx; i < p->hand_count - 1; i++) {
      p->hand[i] = p->hand[i + 1];
    }
    p->hand_count--;
  }
}

/**
 * @brief Retire tile from player.
 *
 * @param p Parameter p.
 * @param tile_id Parameter tile_id.
 * @return Result value.
 */
int remove_tile_from_player(Player *p, int tile_id) {
  int before = p->hand_count;
  remove_tile_from_hand(p, tile_id);
  return p->hand_count != before;
}

/**
 * @brief Calcule hand penalty.
 *
 * @param p Parameter p.
 * @return Result value.
 */
int calculate_hand_penalty(Player *p) {
  if (!p) {
    return 0;
  }
  int penalty = 0;
  for (int i = 0; i < p->hand_count; i++) {
    penalty += (p->hand[i].is_joker) ? 30 : p->hand[i].value;
  }
  return penalty;
}

/**
 * @brief Trie player hand.
 *
 * @param p Parameter p.
 * @param by_color Parameter by_color.
 * @return None.
 */
void sort_player_hand(Player *p, int by_color) {
  if (p->hand_count < 2)
    return;

  for (int i = 0; i < p->hand_count - 1; i++) {
    for (int j = 0; j < p->hand_count - i - 1; j++) {
      int swap = 0;

      if (by_color) {
        // TRI PAR COULEUR (Priorité Couleur, puis Valeur)
        if (p->hand[j].color > p->hand[j + 1].color) {
          swap = 1;
        } else if (p->hand[j].color == p->hand[j + 1].color &&
                   p->hand[j].value > p->hand[j + 1].value) {
          swap = 1;
        }
      } else {
        // TRI PAR VALEUR (Priorité Valeur, puis Couleur)
        if (p->hand[j].value > p->hand[j + 1].value) {
          swap = 1;
        } else if (p->hand[j].value == p->hand[j + 1].value &&
                   p->hand[j].color > p->hand[j + 1].color) {
          swap = 1;
        }
      }

      if (swap) {
        Tile temp = p->hand[j];
        p->hand[j] = p->hand[j + 1];
        p->hand[j + 1] = temp;
      }
    }
  }
}

/**
 * @brief Recherche une tuile par id dans la main d'un joueur.
 *
 * @param p Parameter p.
 * @param id Parameter id.
 * @return Result value.
 */
Tile find_tile(Player *p, int id) {
  for (int i = 0; i < p->hand_count; i++) {
    if (p->hand[i].id == id)
      return p->hand[i];
  }
  return (Tile){-1, 0, NOIR, 0}; // Retourne un ID -1 si non trouvé
}

/**
 * @brief Ajoute to table.
 *
 * @param t Parameter t.
 * @param c Parameter c.
 * @return None.
 */
void add_to_table(Table *t, Combinaison c) {
  if (t->count < MAX_COMB) {
    // On alloue de la mémoire pour stocker les tuiles dans la table
    t->table[t->count].tiles = malloc(sizeof(Tile) * c.count);
    for (int i = 0; i < c.count; i++) {
      t->table[t->count].tiles[i] = c.tiles[i];
    }
    t->table[t->count].count = c.count;
    t->table[t->count].type = c.type;
    t->count++;
  }
}

/**
 * @brief Libere players.
 *
 * @param players Parameter players.
 * @param n Parameter n.
 * @return None.
 */
void free_players(Player players[], int n) {
  for (int i = 0; i < n; i++) {
    free(players[i].hand);
    free(players[i].name);
  }
}





