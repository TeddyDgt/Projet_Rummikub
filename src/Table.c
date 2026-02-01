#include "Table.h"

#include "Combinaisons.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialise la structure de la table.
 * Met le compteur de combinaisons à zéro et initialise tous les pointeurs de tuiles à NULL.
 * @param t Pointeur vers la table à initialiser.
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
 * @brief Effectue une copie profonde d'une combinaison.
 * Alloue dynamiquement un nouveau tableau de tuiles et y copie les données de la source.
 * @param src Pointeur vers la combinaison source à copier.
 * @return Une nouvelle structure Combinaison avec sa propre mémoire allouée.
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
 * @brief Ajoute une combinaison à la table de jeu.
 * Utilise une copie profonde pour s'assurer que la table possède sa propre instance des tuiles.
 * Affiche une erreur si la limite MAX_COMB est atteinte.
 * @param t Pointeur vers la table.
 * @param c La combinaison à ajouter.
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
 * @brief Vérifie l'intégrité de l'ensemble du plateau.
 * Parcourt chaque combinaison sur la table et valide si elle respecte toujours les règles.
 * @param t Pointeur vers la table.
 * @return 1 si tout le plateau est valide, 0 si au moins une combinaison est incorrecte.
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
 * @brief Crée un clone complet d'une table existante.
 * Utile pour simuler des coups sans modifier l'état réel du jeu.
 * @param src Pointeur vers la table source.
 * @return Une nouvelle structure Table contenant des copies indépendantes des combinaisons.
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
 * @brief Libère la mémoire de toutes les combinaisons sur la table.
 * Parcourt la table pour libérer chaque tableau de tuiles alloué dynamiquement.
 * @param t Pointeur vers la table à nettoyer.
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
