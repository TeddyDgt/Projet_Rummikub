#ifndef UI_TILES_H
#define UI_TILES_H

#include <stdbool.h>

#include "structs.h"
#include "ui_rect.h"

void ui_draw_tile(Rect r, const Tile *t, bool selected, bool highlight, bool hovered, bool pressed);

#endif
