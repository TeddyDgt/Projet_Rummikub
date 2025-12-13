#ifndef STRUCTURES_H
#define STRUCTURES_H

// Constantes
#define MAX_TILES 106
#define HAND_SIZE_START 14
#define JOKER_VALUE 30 
#define IS_GROUPE 1
#define IS_SUITE 2

// Enumérations pour la lisibilité
typedef enum { NOIR=1, ROUGE=2, BLEU=3, JAUNE=4 } Color;

// Structure d'une Tuile
typedef struct {
    int id;       // Identifiant unique pour le suivi 
    int value;    // 1 à 13, ou code spécial pour Joker
    Color color;  // 1 à 4
    int is_joker; // 1 si c'est un joker, 0 sinon
} Tile;

// Structure d'une Combinaison 
typedef struct {
    Tile *tiles;  // Tableau dynamique de tuiles
    int count;    // Nombre de tuiles dans la combinaison
    int type;     // IS_GROUPE ou IS_SUITE
} Combinaison;

// Structure du Joueur
typedef struct {
    char name[50];
    int score;
    Tile *hand;      // Main du joueur (tableau dynamique)
    int hand_count;  // Nombre de tuiles en main
} Player;

// Structure de la Pioche (Deck)
typedef struct {
    Tile tiles[MAX_TILES];
    int top; // Indice de la prochaine tuile à piocher (ou nombre restant)
} Deck;

#endif