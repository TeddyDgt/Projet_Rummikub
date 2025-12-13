#include "Structure.h"
// Distribue 14 tuiles à chaque joueur au début
void distribute_initial_tiles(Deck *d, Player players[], int num_players);

// Détermine qui commence (celui qui pioche la plus grosse tuile)
int determine_first_player(Deck *d, Player players[], int num_players);

// Vérifie si la partie est terminée (un joueur n'a plus de tuiles ou pioche vide bloquée)
int is_game_over(Player players[], int num_players, Deck *d);

// Calcule les scores finaux selon les règles (Gagnant = somme des autres, Perdants = négatif)
void calculate_final_scores(Player players[], int num_players);

// Sauvegarde les scores/pseudos dans un fichier (Requis par le PDF)
void save_scores_to_file(Player players[], int num_players, char *filename);