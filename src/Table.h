#include "structs.h"
// Initialise le plateau (vide au début)
void init_table(Table *t); // Supposant que tu crées une struct Table contenant un tableau de Combinaisons

// Ajoute une nouvelle combinaison sur la table
void add_combinaison_to_table(Table *t, Combinaison c);

// Vérifie si TOUTES les combinaisons sur la table sont valides
// (À appeler à la fin du tour du joueur)
int verify_whole_table(Table *t);

// Copie l'état de la table (Sauvegarde temporaire au début du tour)
// Si le joueur fait des coups invalides, on restaure cette copie
Table clone_table(Table *src);

// Libère la mémoire de la table (si allocation dynamique)
void free_table(Table *t);