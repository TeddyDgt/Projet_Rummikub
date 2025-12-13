#include "Draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fonction : Initialiser et mélanger la pioche
void init_deck(Deck *d) {
    int index = 0;
    // Création des tuiles (1 à 13, deux fois chaque couleur = 104 + 2 joker = 106)
    for (int set = 0; set < 2; set++) {
        for (int col = 1; col <= 4; col++) {
            for (int val = 1; val <= 13; val++) {
                d->tiles[index].value = val;
                d->tiles[index].color = col;
                d->tiles[index].is_joker = 0;
                d->tiles[index].id = index;
                index++;
            }
        }
    }
    // Ajout des 2 Jokers
    d->tiles[index] = (Tile){index, 0, 0, 1}; index++;
    d->tiles[index] = (Tile){index, 0, 0, 1}; index++;
    
    d->top = MAX_TILES;

    // Mélange (Fisher-Yates)
    srand(time(NULL));
    for (int i = MAX_TILES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Tile temp = d->tiles[i];
        d->tiles[i] = d->tiles[j];
        d->tiles[j] = temp;
    }
}

// Fonction : Piocher une tuile
Tile draw_tile(Deck *d) {
    if (d->top <= 0) {
        printf("Pioche vide !\n");
        return (Tile){0,0,0,0}; // Tuile vide d'erreur
    }
    d->top--;
    return d->tiles[d->top];
}