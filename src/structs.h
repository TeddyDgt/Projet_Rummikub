#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_TILES 106
#define HAND_SIZE_START 14
#define MAX_COMB 100

// Constantes pour le type de combinaison
#define IS_GROUPE 1
#define IS_SUITE 2

typedef enum { NOIR = 1, ROUGE = 2, BLEU = 3, JAUNE = 4 } Color;

typedef struct {
  int id;
  int value;
  Color color;
  int is_joker;
} Tile;

typedef struct {
  Tile *tiles;
  int count;
  int type; // <-- REAJOUTÉ ICI : 0 (vide), IS_GROUPE ou IS_SUITE
} Combinaison;

typedef struct {
  char *name;
  int score;
  Tile *hand;
  int hand_count;
} Player;

typedef struct {
  Tile tiles[MAX_TILES];
  int top;
} Deck;

typedef struct {
  Combinaison table[MAX_COMB];
  int count;
} Table;

#endif