#include "ui_rect.h"

Rect rect_make(float x, float y, float w, float h) {
    Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

int point_in_rect(float px, float py, Rect r) {
    return px >= r.x && px <= (r.x + r.w) && py >= r.y && py <= (r.y + r.h);
}
