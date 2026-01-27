#ifndef UI_RECT_H
#define UI_RECT_H

typedef struct {
    float x;
    float y;
    float w;
    float h;
} Rect;

Rect rect_make(float x, float y, float w, float h);
int point_in_rect(float px, float py, Rect r);

#endif
