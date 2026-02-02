#include "ui_menu.h"

#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"

#include "audio.h"
#include "Draw.h"
#include "GameLoop.h"
#include "Players.h"
#include "Table.h"

#include "ui_input.h"
#include "ui_persist.h"
#include "ui_rect.h"
#include "ui_text.h"
#include "ui_util.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Realise l'operation menu_apply_backspace.
 *
 * @param game Parameter game.
 * @return None.
 */
static void menu_apply_backspace(GuiGame *game) {
    if (game->menu_selected_name < 0 || game->menu_selected_name >= 4) {
        return;
    }
    char *name = game->menu_player_names[game->menu_selected_name];
    size_t len = strlen(name);
    if (len == 0) {
        return;
    }
    name[len - 1] = '\0';
}

/**
 * @brief Realise l'operation menu_append_char.
 *
 * @param game Parameter game.
 * @param c Parameter c.
 * @return None.
 */
static void menu_append_char(GuiGame *game, char c) {
    if (game->menu_selected_name < 0 || game->menu_selected_name >= 4) {
        return;
    }
    char *name = game->menu_player_names[game->menu_selected_name];
    size_t len = strlen(name);
    if (len + 1 >= sizeof(game->menu_player_names[0])) {
        return;
    }
    name[len] = c;
    name[len + 1] = '\0';
}

/**
 * @brief Realise l'operation menu_handle_keyboard.
 *
 * @param game Parameter game.
 * @param w Parameter w.
 * @return None.
 */
static void menu_handle_keyboard(GuiGame *game, GuiWindow *w) {
    if (game->menu_selected_name < 0) {
        return;
    }

    if (ui_key_pressed(game, w, GLFW_KEY_BACKSPACE)) {
        menu_apply_backspace(game);
    }

    if (ui_key_pressed(game, w, GLFW_KEY_SPACE)) {
        menu_append_char(game, ' ');
    }

    for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; key++) {
        if (ui_key_pressed(game, w, key)) {
            menu_append_char(game, (char)('A' + (key - GLFW_KEY_A)));
        }
    }

    for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; key++) {
        if (ui_key_pressed(game, w, key)) {
            menu_append_char(game, (char)('0' + (key - GLFW_KEY_0)));
        }
    }
}

/**
 * @brief Realise l'operation menu_start_match.
 *
 * @param game Parameter game.
 * @return None.
 */
static void menu_start_match(GuiGame *game) {
    if (game->players_initialized) {
        free_players(game->players, game->num_players);
        game->players_initialized = false;
    }
    free_table(&game->table);
    free_table(&game->table_backup);

    init_deck(&game->deck);
    init_table(&game->table);

    game->num_players = game->menu_player_count;
    for (int i = 0; i < game->num_players; i++) {
        char fallback[32];
        const char *name = game->menu_player_names[i];
        if (!name[0]) {
            snprintf(fallback, sizeof(fallback), "Joueur %d", i + 1);
            name = fallback;
        }
        init_player(&game->players[i], (char *)name);
        game->players[i].is_ai = game->menu_is_ai[i] ? 1 : 0;
        game->players[i].has_initial_meld = 0;
    }
    game->players_initialized = true;
    ui_persist_save_players(game->menu_player_names, game->menu_is_ai, game->menu_player_count);

    game->current_player = determine_first_player(&game->deck, game->num_players);
    distribute_initial_tiles(&game->deck, game->players, game->num_players);

    game->table_backup = clone_table(&game->table);
    ui_clear_selection(game->selected, MAX_TILES);

    game->active_comb_index = -1;
    game->last_player = game->current_player;
    game->last_hand_count = game->players[game->current_player].hand_count;
    if (game->current_player >= 0 && game->current_player < game->num_players) {
        Player *p = &game->players[game->current_player];
        memcpy(game->hand_backup, p->hand, sizeof(Tile) * (size_t)p->hand_count);
        game->hand_backup_count = p->hand_count;
    }
    game->turn_points = 0;
    game->turn_played = false;
    game->drag_pending = false;
    game->dragging = false;
    game->drag_source = 0;
    game->drag_candidate_index = -1;
    game->drag_hand_index = -1;
    game->drag_candidate_table_comb = -1;
    game->drag_candidate_table_index = -1;
    game->drag_table_comb = -1;
    game->drag_table_index = -1;
    game->drag_w = 0.0f;
    game->drag_h = 0.0f;
    game->notification[0] = '\0';
    game->notification_timer = 0.0f;
    game->notification_kind = 0;
    game->last_time = 0.0;
    ui_input_reset(game);
    game->state = GUI_STATE_MATCH;
}

/**
 * @brief Rend l'ecran de menu et gere les interactions.
 *
 * @param game Parameter game.
 * @param w Parameter w.
 * @param fb_w Parameter fb_w.
 * @param fb_h Parameter fb_h.
 * @return None.
 */
void ui_menu_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h) {
    Rect panel = rect_make(fb_w * 0.2f, fb_h * 0.15f, fb_w * 0.6f, fb_h * 0.7f);
    Rect title = rect_make(panel.x, panel.y + 20.0f, panel.w, 40.0f);

    Rect count_row = rect_make(panel.x + 40.0f, panel.y + 90.0f, panel.w - 80.0f, 50.0f);
    Rect minus_btn = rect_make(count_row.x, count_row.y, 40.0f, count_row.h);
    Rect plus_btn = rect_make(count_row.x + count_row.w - 40.0f, count_row.y, 40.0f, count_row.h);
    Rect count_label = rect_make(count_row.x + 60.0f, count_row.y, count_row.w - 120.0f, count_row.h);

    Rect names_panel = rect_make(panel.x + 40.0f, panel.y + 160.0f, panel.w - 80.0f, 220.0f);
    Rect howto_btn = rect_make(panel.x + 140.0f, panel.y + panel.h - 140.0f, panel.w - 280.0f, 42.0f);
    Rect create_btn = rect_make(panel.x + 140.0f, panel.y + panel.h - 80.0f, panel.w - 280.0f, 50.0f);

    double mx = 0.0, my = 0.0;
    gui_get_mouse_pos(w, &mx, &my);
    double now = gui_get_time_seconds();
    bool mouse_down = gui_mouse_button_down(w, GUI_MOUSE_LEFT);
    bool mouse_clicked = mouse_down && !game->prev_mouse_down;
    bool howto_ready = now >= game->howto_cooldown_until;

    bool hover_minus = point_in_rect((float)mx, (float)my, minus_btn);
    bool hover_plus = point_in_rect((float)mx, (float)my, plus_btn);
    bool hover_create = point_in_rect((float)mx, (float)my, create_btn);
    bool hover_howto = point_in_rect((float)mx, (float)my, howto_btn);

    r2d_fill_rect(0.0f, 0.0f, (float)fb_w, (float)fb_h, 0.06f, 0.07f, 0.08f, 1.0f);
    r2d_fill_rect(panel.x, panel.y, panel.w, panel.h, 0.12f, 0.13f, 0.14f, 1.0f);
    r2d_stroke_rect(panel.x, panel.y, panel.w, panel.h, 0.35f, 0.35f, 0.35f, 1.0f, 2.0f);

    ui_draw_text_centered(title, 3.0f, "RUMMIKUB", 0.92f, 0.92f, 0.92f, 1.0f);

    r2d_stroke_rect(count_row.x, count_row.y, count_row.w, count_row.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);
    {
        float br = hover_minus ? 0.22f : 0.18f;
        float bg = hover_minus ? 0.22f : 0.18f;
        float bb = hover_minus ? 0.24f : 0.20f;
        if (mouse_down && hover_minus) {
            br *= 0.85f; bg *= 0.85f; bb *= 0.85f;
        }
        r2d_fill_rect(minus_btn.x, minus_btn.y, minus_btn.w, minus_btn.h, br, bg, bb, 1.0f);
    }
    {
        float br = hover_plus ? 0.22f : 0.18f;
        float bg = hover_plus ? 0.22f : 0.18f;
        float bb = hover_plus ? 0.24f : 0.20f;
        if (mouse_down && hover_plus) {
            br *= 0.85f; bg *= 0.85f; bb *= 0.85f;
        }
        r2d_fill_rect(plus_btn.x, plus_btn.y, plus_btn.w, plus_btn.h, br, bg, bb, 1.0f);
    }

    ui_draw_text_centered(minus_btn, 2.0f, "-", 0.90f, 0.90f, 0.90f, 1.0f);
    ui_draw_text_centered(plus_btn, 2.0f, "+", 0.90f, 0.90f, 0.90f, 1.0f);

    {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "JOUEURS %d", game->menu_player_count);
        ui_draw_text_centered(count_label, 2.0f, buffer, 0.85f, 0.85f, 0.85f, 1.0f);
    }

    float row_h = 44.0f;
    float row_gap = 12.0f;
    for (int i = 0; i < game->menu_player_count; i++) {
        Rect row = rect_make(names_panel.x,
                             names_panel.y + (float)i * (row_h + row_gap),
                             names_panel.w,
                             row_h);
        Rect ai_btn = rect_make(row.x + row.w - 80.0f, row.y + 6.0f, 70.0f, row.h - 12.0f);
        bool hover_row = point_in_rect((float)mx, (float)my, row);
        float bg = (i == game->menu_selected_name) ? 0.20f : 0.14f;
        if (hover_row) {
            bg += 0.04f;
        }
        r2d_fill_rect(row.x, row.y, row.w, row.h, bg, bg + 0.02f, bg + 0.03f, 1.0f);
        r2d_stroke_rect(row.x, row.y, row.w, row.h, 0.40f, 0.40f, 0.40f, 1.0f, 2.0f);
        ui_draw_text(row.x + 12.0f, row.y + 10.0f, 1.6f, game->menu_player_names[i],
                     0.90f, 0.90f, 0.90f, 1.0f);

        bool hover_ai = point_in_rect((float)mx, (float)my, ai_btn);
        if (game->menu_is_ai[i]) {
            float ar = hover_ai ? 0.26f : 0.20f;
            float ag = hover_ai ? 0.34f : 0.28f;
            float ab = hover_ai ? 0.44f : 0.38f;
            if (mouse_down && hover_ai) {
                ar *= 0.85f; ag *= 0.85f; ab *= 0.85f;
            }
            r2d_fill_rect(ai_btn.x, ai_btn.y, ai_btn.w, ai_btn.h, ar, ag, ab, 1.0f);
            r2d_stroke_rect(ai_btn.x, ai_btn.y, ai_btn.w, ai_btn.h, 0.50f, 0.70f, 0.90f, 1.0f, 2.0f);
            ui_draw_text_centered(ai_btn, 1.3f, "IA", 0.92f, 0.92f, 0.92f, 1.0f);
        } else {
            float ar = hover_ai ? 0.22f : 0.18f;
            float ag = hover_ai ? 0.22f : 0.18f;
            float ab = hover_ai ? 0.22f : 0.18f;
            if (mouse_down && hover_ai) {
                ar *= 0.85f; ag *= 0.85f; ab *= 0.85f;
            }
            r2d_fill_rect(ai_btn.x, ai_btn.y, ai_btn.w, ai_btn.h, ar, ag, ab, 1.0f);
            r2d_stroke_rect(ai_btn.x, ai_btn.y, ai_btn.w, ai_btn.h, 0.45f, 0.45f, 0.45f, 1.0f, 2.0f);
            ui_draw_text_centered(ai_btn, 1.1f, "HUM", 0.90f, 0.90f, 0.90f, 1.0f);
        }
    }

    {
        float br = hover_howto ? 0.22f : 0.18f;
        float bg = hover_howto ? 0.26f : 0.22f;
        float bb = hover_howto ? 0.30f : 0.26f;
        if (mouse_down && hover_howto) {
            br *= 0.85f; bg *= 0.85f; bb *= 0.85f;
        }
        r2d_fill_rect(howto_btn.x, howto_btn.y, howto_btn.w, howto_btn.h, br, bg, bb, 1.0f);
        r2d_stroke_rect(howto_btn.x, howto_btn.y, howto_btn.w, howto_btn.h, 0.55f, 0.55f, 0.55f, 1.0f, 2.0f);
        ui_draw_text_centered(howto_btn, 1.3f, "COMMENT JOUER ?", 0.94f, 0.94f, 0.94f, 1.0f);
    }

    {
        float br = hover_create ? 0.26f : 0.20f;
        float bg = hover_create ? 0.36f : 0.30f;
        float bb = hover_create ? 0.26f : 0.20f;
        if (mouse_down && hover_create) {
            br *= 0.85f; bg *= 0.85f; bb *= 0.85f;
        }
        r2d_fill_rect(create_btn.x, create_btn.y, create_btn.w, create_btn.h, br, bg, bb, 1.0f);
        r2d_stroke_rect(create_btn.x, create_btn.y, create_btn.w, create_btn.h, 0.60f, 0.80f, 0.60f, 1.0f, 2.0f);
        ui_draw_text_centered(create_btn, 2.0f, "CREER UNE PARTIE", 0.95f, 0.95f, 0.95f, 1.0f);
    }

    game->prev_mouse_down = mouse_down;

    if (mouse_clicked) {
        if (point_in_rect((float)mx, (float)my, minus_btn)) {
            if (game->menu_player_count > 2) {
                game->menu_player_count--;
                if (game->menu_selected_name >= game->menu_player_count) {
                    game->menu_selected_name = game->menu_player_count - 1;
                }
            }
            audio_play_sfx(AUDIO_SFX_CLICK);
        } else if (point_in_rect((float)mx, (float)my, plus_btn)) {
            if (game->menu_player_count < 4) {
                game->menu_player_count++;
                game->menu_is_ai[game->menu_player_count - 1] = false;
            }
            audio_play_sfx(AUDIO_SFX_CLICK);
        } else if (howto_ready && point_in_rect((float)mx, (float)my, howto_btn)) {
            audio_play_sfx(AUDIO_SFX_CLICK);
            game->howto_return_state = GUI_STATE_MENU;
            game->state = GUI_STATE_HOWTO;
            game->howto_cooldown_until = now + 0.25;
            ui_input_reset(game);
        } else if (point_in_rect((float)mx, (float)my, create_btn)) {
            audio_play_sfx(AUDIO_SFX_PLAY);
            menu_start_match(game);
        } else {
            for (int i = 0; i < game->menu_player_count; i++) {
                Rect row = rect_make(names_panel.x,
                                     names_panel.y + (float)i * (row_h + row_gap),
                                     names_panel.w,
                                     row_h);
                Rect ai_btn = rect_make(row.x + row.w - 80.0f, row.y + 6.0f, 70.0f, row.h - 12.0f);
                if (point_in_rect((float)mx, (float)my, ai_btn)) {
                    game->menu_is_ai[i] = !game->menu_is_ai[i];
                    audio_play_sfx(AUDIO_SFX_CLICK);
                    break;
                }
                if (point_in_rect((float)mx, (float)my, row)) {
                    game->menu_selected_name = i;
                    audio_play_sfx(AUDIO_SFX_CLICK);
                    break;
                }
            }
        }
    }

    menu_handle_keyboard(game, w);
}





