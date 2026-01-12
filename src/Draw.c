#include "Draw.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

void shuffle(Tile *array, unsigned int n) {
  if (n > 1) {
    for (unsigned int i = 0; i < n - 1; i++) {
      unsigned int j = i + rand() % (n - i);
      Tile t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

void init_deck(Deck *d) {
  int index = 0;
  for (int set = 0; set < 2; set++) { // Deux set de chaque couleur
    for (int color = 1; color <= 4;
         color++) { // Les 4 couleurs allant de NOIR à JAUNE
      for (int val = 1; val <= 13; val++) { // Les valeurs des tuiles de 1 à 13
        d->tiles[index].value = val;
        d->tiles[index].color = (Color)color;
        d->tiles[index].is_joker = 0;
        d->tiles[index].id = index;
        index++;
      }
    }
  }

  // Ajout des 2 Jokers
  d->tiles[index] = (Tile){index, 0, NOIR, 1};
  index++;
  d->tiles[index] = (Tile){index, 0, ROUGE, 1};
  index++;

  d->top = MAX_TILES;

  shuffle(d->tiles, MAX_TILES);
}

Tile draw_tile(Deck *d) {
  if (d->top <= 0) {
    printf("Pioche vide !\n");
    return (Tile){0, 0, NOIR,
                  0}; // Tuile vide d'erreur (à enlever certainement)
  }
  d->top--;
  return d->tiles[d->top];
}