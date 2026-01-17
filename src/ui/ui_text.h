#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "ui_rect.h"

float ui_text_width(const char *text, float scale);
float ui_text_height(float scale);

void ui_draw_text(float x, float y, float scale, const char *text,
                  float r, float g, float b, float a);

void ui_draw_text_centered(Rect r, float scale, const char *text,
                           float cr, float cg, float cb, float ca);

#endif
