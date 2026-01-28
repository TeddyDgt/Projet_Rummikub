#include "gui_screen.h"

#include "ui_match.h"
#include "ui_menu.h"
#include "ui_gameover.h"

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

    ui_match_render(game, w, fb_w, fb_h);
}
