#ifndef PIOCHE_H
#define PIOCHE_H

#include "structs.h"

void init_deck(Deck *d);
Tile draw_tile(Deck *d);
void shuffle(Tile *array, unsigned int n);

// int is_deck_empty(Deck *d);

#endif