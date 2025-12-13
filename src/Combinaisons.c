#include "Structure.h"
#include "Combinaisons.h"
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