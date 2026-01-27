#ifndef UI_PERSIST_H
#define UI_PERSIST_H

#include <stdbool.h>

bool ui_persist_load_players(char names[4][16], bool is_ai[4], int *count);
void ui_persist_save_players(const char names[4][16], const bool is_ai[4], int count);

#endif
