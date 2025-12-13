#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "structure.h"

// Prototypes des fonctions
void init_deck(Deck *d);
Tile draw_tile(Deck *d);
int is_valid_group(Combinaison *c);
int is_valid_suite(Combinaison *c);

#endif