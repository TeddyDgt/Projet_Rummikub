#include "ui_rect.h"

/**
 * @brief Crée une structure Rect initialisée.
 * @param x Position horizontale.
 * @param y Position verticale.
 * @param w Largeur.
 * @param h Hauteur.
 * @return Une structure Rect remplie.
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
 * @brief Vérifie si un point se trouve à l'intérieur d'un rectangle.
 * Utilisé principalement pour le picking (souris sur bouton/tuile).
 * @param px Coordonnée X du point.
 * @param py Coordonnée Y du point.
 * @param r Rectangle cible.
 * @return 1 si le point est dedans, 0 sinon.
 */
int point_in_rect(float px, float py, Rect r) {
    return px >= r.x && px <= (r.x + r.w) && py >= r.y && py <= (r.y + r.h);
}
