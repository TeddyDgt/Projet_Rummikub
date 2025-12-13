#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_logic.h"
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

// Fonction : Vérifier un GROUPE (3 ou 4 mêmes valeurs, couleurs différentes)
int is_valid_group(Combinaison *c) {
    if (c->count < 3 || c->count > 4) return 0;

    int ref_val = -1;
    int colors[5] = {0}; 

    for (int i = 0; i < c->count; i++) {
        if (c->tiles[i].is_joker) continue; // On saute le joker pour l'instant

        if (ref_val == -1) ref_val = c->tiles[i].value;
        else if (c->tiles[i].value != ref_val) return 0; // Valeur différente

        if (colors[c->tiles[i].color]) return 0; // Couleur déjà présente
        colors[c->tiles[i].color] = 1;
    }
    return 1;
}

// Fonction : Vérifier une SUITE (Même couleur, valeurs qui se suivent)
int is_valid_suite(Combinaison *c) {
    if (c->count < 3) return 0;
    // NOTE : On suppose ici que c->tiles est déjà trié
    
    int ref_col = -1;
    for (int i = 0; i < c->count; i++) {
        if (c->tiles[i].is_joker) continue;

        if (ref_col == -1) ref_col = c->tiles[i].color;
        else if (c->tiles[i].color != ref_col) return 0; // Couleur différente

        if (i > 0 && !c->tiles[i-1].is_joker) {
            if (c->tiles[i].value != c->tiles[i-1].value + 1) return 0; // Pas consécutif
        }
    }
    return 1;
}