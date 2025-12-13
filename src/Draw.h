#ifndef PIOCHE_H
#define PIOCHE_H

#include "structs.h" // Il a besoin de connaître Tile et Deck

// Juste les prototypes ici
void init_deck(Deck *d);
Tile draw_tile(Deck *d);
int is_deck_empty(Deck *d);

#endif