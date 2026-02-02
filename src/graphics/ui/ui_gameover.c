#include "ui_gameover.h"

#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"

#include "audio.h"
#include "Players.h"
#include "Table.h"
#include "ui_input.h"
#include "ui_rect.h"
#include "ui_text.h"
#include "ui_util.h"

#include <stdio.h>

/**
 * @brief Realise l'operation gameover_to_menu.
 *
 * @param game Parameter game.
 * @return None.
 */
static void gameover_to_menu(GuiGame *game) {
    if (!game) {
        return;
    }

    if (game->players_initialized) {
        free_players(game->players, game->num_players);
        game->players_initialized = false;
    }
    free_table(&game->table);
    free_table(&game->table_backup);
    init_table(&game->table);
    init_table(&game->table_backup);

    game->active_comb_index = -1;
    game->turn_points = 0;
    game->turn_played = false;
    game->menu_selected_name = 0;
    ui_input_reset(game);
    game->state = GUI_STATE_MENU;
}

/**
 * @brief Rend l'ecran de fin de partie.
 *
 * @param game Parameter game.
 * @param w Parameter w.
 * @param fb_w Parameter fb_w.
 * @param fb_h Parameter fb_h.
 * @return None.
 */
void ui_gameover_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h) {
    if (!game || !w) {
        return;
    }

    Rect panel = rect_make(fb_w * 0.18f, fb_h * 0.12f, fb_w * 0.64f, fb_h * 0.76f);
    Rect title = rect_make(panel.x, panel.y + 20.0f, panel.w, 40.0f);
    Rect list = rect_make(panel.x + 60.0f, panel.y + 90.0f, panel.w - 120.0f, panel.h - 190.0f);
    Rect continue_btn = rect_make(panel.x + panel.w * 0.32f, panel.y + panel.h - 70.0f, panel.w * 0.36f, 46.0f);

    r2d_fill_rect(0.0f, 0.0f, (float)fb_w, (float)fb_h, 0.06f, 0.07f, 0.08f, 1.0f);
    r2d_fill_rect(panel.x, panel.y, panel.w, panel.h, 0.12f, 0.13f, 0.14f, 1.0f);
    r2d_stroke_rect(panel.x, panel.y, panel.w, panel.h, 0.35f, 0.35f, 0.35f, 1.0f, 2.0f);

    ui_draw_text_centered(title, 2.6f, "FIN DE PARTIE", 0.95f, 0.95f, 0.95f, 1.0f);

    int order[4];
    int count = game->num_players;
    for (int i = 0; i < count; i++) {
        order[i] = i;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (game->players[order[j]].score > game->players[order[i]].score) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
    }

    float row_h = 46.0f;
    float row_gap = 12.0f;
    for (int i = 0; i < count; i++) {
        Rect row = rect_make(list.x,
                             list.y + i * (row_h + row_gap),
                             list.w,
                             row_h);
        int pidx = order[i];
        float base = (i == 0) ? 0.18f : 0.12f;
        r2d_fill_rect(row.x, row.y, row.w, row.h, base, base + 0.03f, base + 0.05f, 1.0f);
        r2d_stroke_rect(row.x, row.y, row.w, row.h, 0.45f, 0.45f, 0.45f, 1.0f, 2.0f);

        char label[128];
        snprintf(label, sizeof(label), "%d. %s", i + 1, game->players[pidx].name);
        ui_draw_text(row.x + 16.0f, row.y + 12.0f, 1.4f, label, 0.95f, 0.95f, 0.95f, 1.0f);

        char score[64];
        snprintf(score, sizeof(score), "%d", game->players[pidx].score);
        float score_w = ui_text_width(score, 1.3f);
        ui_draw_text(row.x + row.w - score_w - 16.0f, row.y + 12.0f, 1.3f, score,
                     0.90f, 0.90f, 0.90f, 1.0f);
    }

    double mx = 0.0, my = 0.0;
    gui_get_mouse_pos(w, &mx, &my);
    bool mouse_down = gui_mouse_button_down(w, GUI_MOUSE_LEFT);
    bool mouse_clicked = mouse_down && !game->prev_mouse_down;
    bool hover_continue = point_in_rect((float)mx, (float)my, continue_btn);

    float br = hover_continue ? 0.26f : 0.20f;
    float bg = hover_continue ? 0.36f : 0.30f;
    float bb = hover_continue ? 0.26f : 0.20f;
    if (mouse_down && hover_continue) {
        br *= 0.85f;
        bg *= 0.85f;
        bb *= 0.85f;
    }
    r2d_fill_rect(continue_btn.x, continue_btn.y, continue_btn.w, continue_btn.h, br, bg, bb, 1.0f);
    r2d_stroke_rect(continue_btn.x, continue_btn.y, continue_btn.w, continue_btn.h, 0.60f, 0.80f, 0.60f, 1.0f, 2.0f);
    ui_draw_text_centered(continue_btn, 1.6f, "CONTINUER", 0.96f, 0.96f, 0.96f, 1.0f);
    game->prev_mouse_down = mouse_down;

    if (mouse_clicked && hover_continue) {
        audio_play_sfx(AUDIO_SFX_CLICK);
        gameover_to_menu(game);
    }
}





