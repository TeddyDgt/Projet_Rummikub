#include "Players.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *color_to_str(Color c) {
  switch (c) {
  case NOIR:
    return "NOIR";
  case ROUGE:
    return "ROUGE";
  case BLEU:
    return "BLEU";
  case JAUNE:
    return "JAUNE";
  default:
    return "?";
  }
}

int cmp_by_value_then_color(const void *a, const void *b) {
  const Tile *ta = (const Tile *)a;
  const Tile *tb = (const Tile *)b;

  // Jokers at the end
  if (ta->is_joker != tb->is_joker)
    return ta->is_joker - tb->is_joker;

  if (ta->value != tb->value)
    return ta->value - tb->value;
  return (int)ta->color - (int)tb->color;
}

void sort_by_value(Tile *hand, int length) {
  Tile *temp_hand = (Tile *)malloc(length * sizeof(Tile));
  if (!temp_hand)
    return;

  memcpy(temp_hand, hand, length * sizeof(Tile));

  for (int i = 0; i < length - 1; i++) {
    int min_idx = i;
    for (int j = i + 1; j < length; j++) {
      if (temp_hand[j].value < temp_hand[min_idx].value) {
        min_idx = j;
      }
    }
    Tile tmp = temp_hand[i];
    temp_hand[i] = temp_hand[min_idx];
    temp_hand[min_idx] = tmp;
  }

  memcpy(hand, temp_hand, length * sizeof(Tile));
  free(temp_hand);
}

void sort_by_color(Tile *hand, int length) {
  Tile *temp_hand = (Tile *)malloc(length * sizeof(Tile));
  if (!temp_hand)
    return;

  memcpy(temp_hand, hand, length * sizeof(Tile));

  for (int i = 0; i < length - 1; i++) {
    int min_idx = i;
    for (int j = i + 1; j < length; j++) {
      if (temp_hand[j].color < temp_hand[min_idx].color) {
        min_idx = j;
      }
    }
    Tile tmp = temp_hand[i];
    temp_hand[i] = temp_hand[min_idx];
    temp_hand[min_idx] = tmp;
  }

  memcpy(hand, temp_hand, length * sizeof(Tile));
  free(temp_hand);
}

int cmp_by_color_then_value(const void *a, const void *b) {
  const Tile *ta = (const Tile *)a;
  const Tile *tb = (const Tile *)b;

  // Jokers at the end
  if (ta->is_joker != tb->is_joker)
    return ta->is_joker - tb->is_joker;

  if ((int)ta->color != (int)tb->color)
    return (int)ta->color - (int)tb->color;
  return ta->value - tb->value;
}

void init_player(Player *p, char *nom) {
  p->score = 0;
  p->hand = NULL;
  p->hand_count = 0;
  p->name = nom;
}

void add_tile_to_player(Player *p, Tile t) {
  Tile *new_hand =
      (Tile *)realloc(p->hand, (size_t)(p->hand_count + 1) * sizeof(Tile));
  if (!new_hand) {
    fprintf(stderr, "Le realloc de la main du joueur n'a pas fonctionné\n");
    return;
  }
  p->hand = new_hand;
  p->hand[p->hand_count] = t;
  p->hand_count++;
}

int remove_tile_from_player(Player *p, int tile_id) {
  if (!p || !p->hand || p->hand_count <= 0)
    return 0;

  int idx = -1;
  for (int i = 0; i < p->hand_count; i++) {
    if (p->hand[i].id == tile_id) {
      idx = i;
      break;
    }
  }
  if (idx < 0)
    return 0;

  // Swap with last, shrink count
  p->hand[idx] = p->hand[p->hand_count - 1];
  p->hand_count--;

  if (p->hand_count == 0) {
    free(p->hand);
    p->hand = NULL;
    return 1;
  }

  Tile *new_hand =
      (Tile *)realloc(p->hand, (size_t)p->hand_count * sizeof(Tile));
  if (new_hand) {
    p->hand = new_hand;
  }
  return 1;
}

void sort_player_hand(Player *p, int by_color) {
  if (by_color) {
    sort_by_color(p->hand, p->hand_count);
  } else {
    sort_by_value(p->hand, p->hand_count);
  }
}

int calculate_hand_penalty(Player *p) {
  if (!p || !p->hand)
    return 0;
  int sum = 0;
  for (int i = 0; i < p->hand_count; i++) {
    if (p->hand[i].is_joker)
      sum += 30; // convention Rummikub: Joker = 30
    else
      sum += p->hand[i].value;
  }
  return sum;
}

void print_player_hand(Player *p) {
  printf("Main de %s (%d tuiles):\n", p->name, p->hand_count);
  for (int i = 0; i < p->hand_count; i++) {
    Tile t = p->hand[i];
    if (t.is_joker) {
      printf("  [id=%d] JOKER (%s)\n", t.id, color_to_str(t.color));
    } else {
      printf("  [id=%d] %d %s\n", t.id, t.value, color_to_str(t.color));
    }
  }
}

void print_combinaisons(Combinaison *c, int length) {
  for (int i = 0; i < length; i++) {
    printf("Combinaison %i :\n", i+1);
    for (int j = 0; j < c->count; j++) {
      if (c[i].tiles[j].is_joker) {
        printf("  [id=%d] JOKER (%s)\n", c[i].tiles[j].id, color_to_str(c[i].tiles[j].color));
      } else {
        printf("  [id=%d] %d %s\n", c[i].tiles[j].id, c[i].tiles[j].value, color_to_str(c[i].tiles[j].color));
      }
    }
    printf("\n");
  }
}

void register_player_move(Player *p, Table *table, Combinaison *combinaisons) {
  // premier tour les combinaisons doivent faire >= 30
}
