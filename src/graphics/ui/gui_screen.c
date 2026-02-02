#include "gui_screen.h"

#include "ui_match.h"
#include "ui_menu.h"
#include "ui_gameover.h"
#include "ui_howto.h"

/**
 * @brief Rend l'ecran correspondant a l'etat du jeu.
 *
 * @param game Parameter game.
 * @param w Parameter w.
 * @param fb_w Parameter fb_w.
 * @param fb_h Parameter fb_h.
 * @return None.
 */
void gui_game_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h) {
    if (!game || !w) {
        return;
    }

    if (game->state == GUI_STATE_MENU) {
        ui_menu_render(game, w, fb_w, fb_h);
        return;
    }

    if (game->state == GUI_STATE_GAMEOVER) {
        ui_gameover_render(game, w, fb_w, fb_h);
        return;
    }

    if (game->state == GUI_STATE_HOWTO) {
        ui_howto_render(game, w, fb_w, fb_h);
        return;
    }

    ui_match_render(game, w, fb_w, fb_h);
}





