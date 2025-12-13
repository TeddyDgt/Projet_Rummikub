#include "Structure.h"
// Vérifie si une combinaison est un Groupe valide (3-4 tuiles, même valeur, couleurs diff)
int is_valid_group(Combinaison *c);

// Vérifie si une combinaison est une Suite valide (3+ tuiles, même couleur, consécutives)
int is_valid_suite(Combinaison *c);

// Vérifie générique : regarde si c'est soit un groupe valide, soit une suite valide
int is_valid_combination(Combinaison *c);

// Fonction utilitaire pour trier les tuiles d'une combinaison (nécessaire pour vérifier la suite)
void sort_combination_tiles(Combinaison *c);