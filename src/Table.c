#include "Table.h"

#include "Combinaisons.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialise table.
 *
 * @param t Parameter t.
 * @return None.
 */
void init_table(Table *t) {
  t->count = 0;
  for (int i = 0; i < MAX_COMB; i++) {
    t->table[i].tiles = NULL;
    t->table[i].count = 0;
    t->table[i].type = 0;
  }
}

/**
 * @brief Realise l'operation deep_copy_combinaison.
 *
 * @param src Parameter src.
 * @return Result value.
 */
static Combinaison deep_copy_combinaison(const Combinaison *src) {
  Combinaison c;
  c.tiles = NULL;
  c.count = 0;
  c.type = 0;

  if (!src || src->count <= 0 || !src->tiles) {
    return c;
  }

  c.count = src->count;
  c.type = src->type;
  c.tiles = (Tile *)malloc((size_t)c.count * sizeof(Tile));
  if (!c.tiles) {
    fprintf(stderr, "[Table] malloc failed while copying combinaison.\n");
    c.count = 0;
    c.type = 0;
    return c;
  }
  memcpy(c.tiles, src->tiles, (size_t)c.count * sizeof(Tile));
  return c;
}

/**
 * @brief Ajoute combinaison to table.
 *
 * @param t Parameter t.
 * @param c Parameter c.
 * @return None.
 */
void add_combinaison_to_table(Table *t, Combinaison c) {
  if (!t)
    return;
  if (t->count >= MAX_COMB) {
    fprintf(stderr, "[Table] MAX_COMB atteint, impossible d'ajouter une combinaison.\n");
    return;
  }

  // Deep copy because 'c.tiles' is dynamic
  Combinaison copy = deep_copy_combinaison(&c);
  t->table[t->count] = copy;
  t->count++;
}

/**
 * @brief Verifie que toutes les combinaisons de la table sont valides.
 *
 * @param t Parameter t.
 * @return Result value.
 */
int verify_whole_table(Table *t) {
  if (!t)
    return 0;
  for (int i = 0; i < t->count; i++) {
    if (!is_valid_combination(&t->table[i])) {
      return 0;
    }
  }
  return 1;
}

/**
 * @brief Duplique table.
 *
 * @param src Parameter src.
 * @return Result value.
 */
Table clone_table(Table *src) {
  Table dst;
  init_table(&dst);

  if (!src)
    return dst;

  dst.count = 0;
  for (int i = 0; i < src->count && i < MAX_COMB; i++) {
    dst.table[i] = deep_copy_combinaison(&src->table[i]);
    dst.count++;
  }
  return dst;
}

/**
 * @brief Libere table.
 *
 * @param t Parameter t.
 * @return None.
 */
void free_table(Table *t) {
  if (!t)
    return;
  for (int i = 0; i < t->count; i++) {
    free(t->table[i].tiles);
    t->table[i].tiles = NULL;
    t->table[i].count = 0;
    t->table[i].type = 0;
  }
  t->count = 0;
}





