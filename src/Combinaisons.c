#include "Combinaisons.h"
#include "./structs.h"
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
  // On suppose ici que c->tiles est déjà trié

  int ref_col = -1;
  for (int i = 0; i < c->count; i++) {
    if (c->tiles[i].is_joker)
      continue;

    if (ref_col == -1)
      ref_col = c->tiles[i].color;
    else if (c->tiles[i].color != (Color)ref_col)
      return 0;

    if (i > 0 && !c->tiles[i - 1].is_joker) {
      if (c->tiles[i].value !=
          c->tiles[i - 1].value + 1) // Si la valeur de la tuile actuelle n'est
                                     // pas égale à la valeur de la dernière + 1
        return 0;
    }
  }
  return 1;
}

int is_valid_combination(Combinaison *c) {
  return is_valid_group(c) || is_valid_suite(c);
}