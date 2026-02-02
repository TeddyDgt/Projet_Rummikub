#include "ui_howto.h"

#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"

#include "audio.h"
#include "ui_input.h"
#include "ui_rect.h"
#include "ui_text.h"
#include "ui_tiles.h"
#include "ui_util.h"

#include <stdio.h>

/**
 * @brief Realise l'operation howto_draw_tile_row.
 *
 * @param x Parameter x.
 * @param y Parameter y.
 * @param tile_w Parameter tile_w.
 * @param tile_h Parameter tile_h.
 * @param tiles Parameter tiles.
 * @param count Parameter count.
 * @return None.
 */
static void howto_draw_tile_row(float x, float y, float tile_w, float tile_h, const Tile *tiles, int count) {
    for (int i = 0; i < count; i++) {
        Rect r = rect_make(x + i * (tile_w + 6.0f), y, tile_w, tile_h);
        ui_draw_tile(r, &tiles[i], false, false, false, false);
    }
}

/**
 * @brief Rend l'ecran Comment jouer et ses exemples.
 *
 * @param game Parameter game.
 * @param w Parameter w.
 * @param fb_w Parameter fb_w.
 * @param fb_h Parameter fb_h.
 * @return None.
 */
void ui_howto_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h) {
    if (!game || !w) {
        return;
    }

    Rect panel = rect_make(fb_w * 0.08f, fb_h * 0.08f, fb_w * 0.84f, fb_h * 0.84f);
    Rect title = rect_make(panel.x, panel.y + 16.0f, panel.w, 40.0f);
    Rect back_btn = rect_make(panel.x + 20.0f, panel.y + 16.0f, 140.0f, 34.0f);

    double mx = 0.0, my = 0.0;
    gui_get_mouse_pos(w, &mx, &my);
    double now = gui_get_time_seconds();
    bool mouse_down = gui_mouse_button_down(w, GUI_MOUSE_LEFT);
    bool mouse_clicked = mouse_down && !game->prev_mouse_down;
    bool hover_back = point_in_rect((float)mx, (float)my, back_btn);
    bool howto_ready = now >= game->howto_cooldown_until;

    r2d_fill_rect(0.0f, 0.0f, (float)fb_w, (float)fb_h, 0.06f, 0.07f, 0.08f, 1.0f);
    r2d_fill_rect(panel.x, panel.y, panel.w, panel.h, 0.12f, 0.13f, 0.14f, 1.0f);
    r2d_stroke_rect(panel.x, panel.y, panel.w, panel.h, 0.35f, 0.35f, 0.35f, 1.0f, 2.0f);

    ui_draw_text_centered(title, 2.2f, "COMMENT JOUER ?", 0.95f, 0.95f, 0.95f, 1.0f);

    float br = hover_back ? 0.26f : 0.20f;
    float bg = hover_back ? 0.36f : 0.30f;
    float bb = hover_back ? 0.26f : 0.20f;
    if (mouse_down && hover_back) {
        br *= 0.85f;
        bg *= 0.85f;
        bb *= 0.85f;
    }
    r2d_fill_rect(back_btn.x, back_btn.y, back_btn.w, back_btn.h, br, bg, bb, 1.0f);
    r2d_stroke_rect(back_btn.x, back_btn.y, back_btn.w, back_btn.h, 0.60f, 0.80f, 0.60f, 1.0f, 2.0f);
    ui_draw_text_centered(back_btn, 1.2f, "RETOUR", 0.96f, 0.96f, 0.96f, 1.0f);

    float left_x = panel.x + 36.0f;
    float right_x = panel.x + panel.w * 0.55f;
    float y = panel.y + 80.0f;
    float line = 28.0f;
    float tile_w = 38.0f;
    float tile_h = 52.0f;

    ui_draw_text(left_x, y, 1.3f, "Combinaisons valides :", 0.92f, 0.92f, 0.92f, 1.0f);
    y += line;

    ui_draw_text(left_x, y, 1.1f, "Suite : 3 tuiles ou + de meme couleur, valeurs consecutives.", 0.86f, 0.86f, 0.86f, 1.0f);
    Tile suite_tiles[] = {
        {0, 5, ROUGE, 0},
        {1, 6, ROUGE, 0},
        {2, 7, ROUGE, 0}
    };
    howto_draw_tile_row(right_x, y - 8.0f, tile_w, tile_h, suite_tiles, 3);
    y += line * 2.0f;

    ui_draw_text(left_x, y, 1.1f, "Groupe : meme valeur, couleurs differentes (3 ou 4 tuiles).", 0.86f, 0.86f, 0.86f, 1.0f);
    Tile group_tiles[] = {
        {3, 9, NOIR, 0},
        {4, 9, ROUGE, 0},
        {5, 9, BLEU, 0},
        {6, 9, JAUNE, 0}
    };
    howto_draw_tile_row(right_x, y - 8.0f, tile_w, tile_h, group_tiles, 4);
    y += line * 2.0f;

    ui_draw_text(left_x, y, 1.1f, "Joker : remplace n'importe quelle tuile.", 0.86f, 0.86f, 0.86f, 1.0f);
    Tile joker_tiles[] = {
        {7, 10, BLEU, 0},
        {8, 0, BLEU, 1},
        {9, 12, BLEU, 0}
    };
    howto_draw_tile_row(right_x, y - 8.0f, tile_w, tile_h, joker_tiles, 3);
    y += line * 2.0f;

    ui_draw_text(left_x, y, 1.1f, "Reorganisation : on peut reprendre une tuile d'une combinaison", 0.86f, 0.86f, 0.86f, 1.0f);
    y += line;
    ui_draw_text(left_x, y, 1.1f, "pour en creer une autre, tant que tout reste valide.", 0.86f, 0.86f, 0.86f, 1.0f);

    Tile source_tiles[] = {
        {10, 3, JAUNE, 0},
        {11, 4, JAUNE, 0},
        {12, 5, JAUNE, 0},
        {13, 6, JAUNE, 0}
    };
    howto_draw_tile_row(right_x, y - 8.0f, tile_w, tile_h, source_tiles, 4);
    y += line * 2.0f;

    ui_draw_text(left_x, y, 1.1f, "Exemple : prendre le 4 pour former 4-4-4.", 0.86f, 0.86f, 0.86f, 1.0f);
    Tile steal_tiles[] = {
        {14, 4, NOIR, 0},
        {15, 4, ROUGE, 0},
        {16, 4, BLEU, 0}
    };
    howto_draw_tile_row(right_x, y - 8.0f, tile_w, tile_h, steal_tiles, 3);

    game->prev_mouse_down = mouse_down;

    if (mouse_clicked && hover_back && howto_ready) {
        audio_play_sfx(AUDIO_SFX_CLICK);
        game->state = game->howto_return_state;
        game->howto_cooldown_until = now + 0.25;
        ui_input_reset(game);
    }
}



