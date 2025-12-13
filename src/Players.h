#include "structs.h"
// Initialise un joueur (nom, score à 0, main vide)
void init_player(Player *p, char *nom);

// Ajoute une tuile dans la main du joueur (après la pioche)
void add_tile_to_player(Player *p, Tile t);

// Retire une tuile de la main du joueur (quand il la pose sur la table)
// Retourne 1 si succès, 0 si le joueur n'avait pas la tuile
int remove_tile_from_player(Player *p, int tile_id);

// Trie la main du joueur (par couleur ou par valeur) - Bonus UX
void sort_player_hand(Player *p, int by_color);

// Calcule la somme des points restants en main (pour le score de fin)
int calculate_hand_penalty(Player *p);

// Affiche la main (pour le mode console/debug)
void print_player_hand(Player *p);