#include "Combinaisons.h"
#include "./structs.h"
#include <stdlib.h>
// On veut un groupe de 3 à 4 tuiles de même valeur mais de couleur différente
int is_valid_group(Combinaison *c) {
  if (c->count < 3 || c->count > 4)
    return 0;

  int ref_val = -1;
  int colors[5] = {0};

  for (int i = 0; i < c->count; i++) {
    if (c->tiles[i].is_joker)
      continue;

    if (ref_val == -1)
      ref_val = c->tiles[i].value;
    else if (c->tiles[i].value != ref_val)
      return 0;

    if (colors[c->tiles[i].color]) // Si la couleur est déjà dans le groupe
      return 0;
    colors[c->tiles[i].color] = 1;
  }
  return 1;
}

// On veut une suite d'au moins 3 tuiles de même couleur et de valeur
// incrémentale (1, 2, 3 etc...)
int is_valid_suite(Combinaison *c) {
  if (c->count < 3)
    return 0;

  int ref_col = -1;
  int first_val = -1;
  int first_val_idx = -1;

  // 1. Trouver la première tuile qui n'est pas un joker pour avoir la référence
  for (int i = 0; i < c->count; i++) {
    if (!c->tiles[i].is_joker) {
      ref_col = c->tiles[i].color;
      first_val = c->tiles[i].value;
      first_val_idx = i;
      break;
    }
  }

  // Si que des jokers (impossible normalement), c'est valide
  if (ref_col == -1)
    return 1;

  // 2. Vérifier que tout le monde a la même couleur et la bonne valeur
  // "virtuelle"
  for (int i = 0; i < c->count; i++) {
    if (c->tiles[i].is_joker)
      continue;

    if ((int)c->tiles[i].color != ref_col)
      return 0;

    // La valeur doit correspondre à sa position par rapport à la première tuile
    // non-joker Exemple : si la tuile [1] est un "5", la tuile [3] doit être un
    // "7" (5 + (3 - 1))
    int expected_val = first_val + (i - first_val_idx);
    if (c->tiles[i].value != expected_val)
      return 0;

    // Les valeurs Rummikub vont de 1 à 13
    if (expected_val < 1 || expected_val > 13)
      return 0;
  }
  return 1;
}

/**
 * @brief Ajoute tile to table comb.
 *
 * @param t Parameter t.
 * @param comb_idx Parameter comb_idx.
 * @param new_tile Parameter new_tile.
 * @return Result value.
 */
int add_tile_to_table_comb(Table *t, int comb_idx, Tile new_tile) {
  if (comb_idx < 0 || comb_idx >= t->count)
    return 0;

  Combinaison *c = &t->table[comb_idx];

  // 1. On agrandit temporairement la combinaison
  c->tiles = realloc(c->tiles, sizeof(Tile) * (c->count + 1));
  c->tiles[c->count] = new_tile;
  c->count++;

  // 2. On trie (important pour les suites)
  // sort_tiles(c->tiles, c->count);

  // 3. On vérifie si c'est toujours valide
  if (is_valid_combination(c)) {
    return 1; // Succès
  } else {
    // 4. Invalide : on annule (on retire la tuile)
    c->count--;
    // Pas strictement besoin de realloc vers le bas tout de suite
    return 0;
  }
}

/**
 * @brief Verifie si valid combination.
 *
 * @param c Parameter c.
 * @return Result value.
 */
int is_valid_combination(Combinaison *c) {
  return is_valid_group(c) || is_valid_suite(c);
}

/**
 * @brief Calcule le score total d'une combinaison.
 *
 * @param c Parameter c.
 * @return Result value.
 */
int combinaison_points(const Combinaison *c) {
  if (!c || c->count <= 0 || !c->tiles) {
    return 0;
  }

  if (is_valid_group((Combinaison *)c)) {
    int ref_val = -1;
    for (int i = 0; i < c->count; i++) {
      if (!c->tiles[i].is_joker) {
        ref_val = c->tiles[i].value;
        break;
      }
    }
    if (ref_val <= 0) {
      return 0;
    }
    return ref_val * c->count;
  }

  if (is_valid_suite((Combinaison *)c)) {
    int ref_col = -1;
    int first_val = -1;
    int first_val_idx = -1;

    for (int i = 0; i < c->count; i++) {
      if (!c->tiles[i].is_joker) {
        ref_col = c->tiles[i].color;
        first_val = c->tiles[i].value;
        first_val_idx = i;
        break;
      }
    }
    if (ref_col == -1 || first_val <= 0) {
      return 0;
    }

    int sum = 0;
    for (int i = 0; i < c->count; i++) {
      int expected_val = first_val + (i - first_val_idx);
      sum += expected_val;
    }
    return sum;
  }

  return 0;
}





