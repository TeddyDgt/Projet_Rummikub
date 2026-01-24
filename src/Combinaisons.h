#include "structs.h"
int is_valid_group(Combinaison *c);

int is_valid_suite(Combinaison *c);

int is_valid_combination(Combinaison *c);

int add_tile_to_table_comb(Table *t, int comb_idx, Tile new_tile);

// à voir plus tard
Combinaison *sort_combination_tiles(Tile *c);

int combinaison_points(const Combinaison *c);
