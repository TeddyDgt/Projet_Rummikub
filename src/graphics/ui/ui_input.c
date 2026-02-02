#include "ui_input.h"

#include "platform/gui_platform.h"

#include <string.h>

/**
 * @brief Détecte si une touche a été pressée (front montant).
 * Compare l'état actuel de la touche avec son état à la frame précédente pour éviter les répétitions.
 * @param game Pointeur vers l'état global pour accéder à l'historique des touches.
 * @param w Fenêtre GUI.
 * @param key Code de la touche à vérifier.
 * @return true si la touche vient d'être pressée, false sinon.
 */
bool ui_key_pressed(GuiGame *game, GuiWindow *w, int key) {
    if (!game || !w) {
        return false;
    }
    if (key < 0 || key >= 512) {
        return false;
    }
    bool down = gui_key_down(w, key);
    bool pressed = down && !game->key_prev[key];
    game->key_prev[key] = down;
    return pressed;
}

/**
 * @brief Réinitialise l'état interne des entrées.
 * Utile lors du changement d'écran pour éviter que des touches pressées ne soient prises en compte par erreur.
 * @param game Pointeur vers l'état global du jeu.
 */
void ui_input_reset(GuiGame *game) {
    if (!game) {
        return;
    }
    game->prev_mouse_down = false;
    memset(game->key_prev, 0, sizeof(game->key_prev));
}
