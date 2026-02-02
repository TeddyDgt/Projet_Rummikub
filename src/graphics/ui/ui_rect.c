#include "ui_rect.h"

/**
 * @brief Construit un rectangle a partir de coordonnees et dimensions.
 *
 * @param x Parameter x.
 * @param y Parameter y.
 * @param w Parameter w.
 * @param h Parameter h.
 * @return Result value.
 */
Rect rect_make(float x, float y, float w, float h) {
    Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

/**
 * @brief Verifie si un point est a l'interieur d'un rectangle.
 *
 * @param px Parameter px.
 * @param py Parameter py.
 * @param r Parameter r.
 * @return Result value.
 */
int point_in_rect(float px, float py, Rect r) {
    return px >= r.x && px <= (r.x + r.w) && py >= r.y && py <= (r.y + r.h);
}





