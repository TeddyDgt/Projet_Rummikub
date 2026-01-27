#include "ui_match.h"

#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"

#include "audio.h"
#include "Draw.h"
#include "Combinaisons.h"
#include "GameLoop.h"
#include "Players.h"
#include "Table.h"

#include "ui_input.h"
#include "ui_rect.h"
#include "ui_text.h"
#include "ui_tiles.h"
#include "ui_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Rect rect;
    int comb_index;
    int tile_index;
} TableTileHit;

enum {
    DRAG_SRC_NONE = 0,
    DRAG_SRC_HAND = 1,
    DRAG_SRC_TABLE = 2
};

enum {
    NOTICE_INFO = 0,
    NOTICE_WARN = 1,
    NOTICE_ERROR = 2
};

static void ui_notify(GuiGame *game, int kind, const char *message) {
    if (!game || !message) {
        return;
    }
    strncpy(game->notification, message, sizeof(game->notification) - 1);
    game->notification[sizeof(game->notification) - 1] = '\0';
    game->notification_kind = kind;
    game->notification_timer = 2.4f;
}

static int clamp_int(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static bool can_edit_table(const GuiGame *game, const Player *p) {
    if (!game || !p) {
        return false;
    }
    return p->has_initial_meld || game->turn_points >= 30;
}

static void table_remove_tile(Table *t, int comb_idx, int tile_idx) {
    if (!t || comb_idx < 0 || comb_idx >= t->count) {
        return;
    }
    Combinaison *c = &t->table[comb_idx];
    if (!c->tiles || tile_idx < 0 || tile_idx >= c->count) {
        return;
    }

    for (int i = tile_idx; i < c->count - 1; i++) {
        c->tiles[i] = c->tiles[i + 1];
    }
    c->count--;

    if (c->count <= 0) {
        free(c->tiles);
        c->tiles = NULL;
        c->count = 0;
        c->type = 0;
        for (int i = comb_idx; i < t->count - 1; i++) {
            t->table[i] = t->table[i + 1];
        }
        t->count--;
        return;
    }
}

static void table_insert_tile(Table *t, int comb_idx, int insert_idx, Tile tile) {
    if (!t || comb_idx < 0 || comb_idx >= t->count) {
        return;
    }
    Combinaison *c = &t->table[comb_idx];
    if (insert_idx < 0) {
        insert_idx = 0;
    }
    if (insert_idx > c->count) {
        insert_idx = c->count;
    }

    Tile *new_tiles = (Tile *)realloc(c->tiles, sizeof(Tile) * (size_t)(c->count + 1));
    if (!new_tiles) {
        return;
    }
    c->tiles = new_tiles;

    for (int i = c->count; i > insert_idx; i--) {
        c->tiles[i] = c->tiles[i - 1];
    }
    c->tiles[insert_idx] = tile;
    c->count++;
}

static int table_add_new_comb(Table *t, Tile tile) {
    if (!t || t->count >= MAX_COMB) {
        return -1;
    }

    Combinaison *c = &t->table[t->count];
    c->tiles = (Tile *)malloc(sizeof(Tile));
    if (!c->tiles) {
        return -1;
    }
    c->tiles[0] = tile;
    c->count = 1;
    c->type = 0;
    t->count++;
    return t->count - 1;
}

static void table_move_tile(Table *t, int from_comb, int from_idx, int to_comb, int to_idx) {
    if (!t) {
        return;
    }
    if (from_comb < 0 || from_comb >= t->count) {
        return;
    }
    if (to_comb < 0 || to_comb >= t->count) {
        return;
    }

    if (from_comb == to_comb) {
        Combinaison *c = &t->table[from_comb];
        if (!c->tiles || from_idx < 0 || from_idx >= c->count) {
            return;
        }
        if (to_idx < 0) {
            to_idx = 0;
        }
        if (to_idx > c->count) {
            to_idx = c->count;
        }
        if (to_idx == from_idx || to_idx == from_idx + 1) {
            return;
        }

        Tile tile = c->tiles[from_idx];
        for (int i = from_idx; i < c->count - 1; i++) {
            c->tiles[i] = c->tiles[i + 1];
        }
        c->count--;
        if (to_idx > from_idx) {
            to_idx--;
        }

        Tile *new_tiles = (Tile *)realloc(c->tiles, sizeof(Tile) * (size_t)(c->count + 1));
        if (!new_tiles) {
            return;
        }
        c->tiles = new_tiles;
        for (int i = c->count; i > to_idx; i--) {
            c->tiles[i] = c->tiles[i - 1];
        }
        c->tiles[to_idx] = tile;
        c->count++;
        return;
    }

    Combinaison *src = &t->table[from_comb];
    if (!src->tiles || from_idx < 0 || from_idx >= src->count) {
        return;
    }
    Tile tile = src->tiles[from_idx];
    int prev_count = t->count;
    table_remove_tile(t, from_comb, from_idx);
    if (t->count < prev_count && from_comb < to_comb) {
        to_comb--;
    }
    if (to_comb < 0 || to_comb >= t->count) {
        return;
    }
    table_insert_tile(t, to_comb, to_idx, tile);
}

static void hand_move_tile(Player *p, bool *selected, int from, int to) {
    if (!p || from == to || from < 0 || to < 0 || from >= p->hand_count || to >= p->hand_count) {
        return;
    }
    Tile temp = p->hand[from];
    bool sel_temp = selected ? selected[from] : false;

    if (from < to) {
        for (int i = from; i < to; i++) {
            p->hand[i] = p->hand[i + 1];
            if (selected) {
                selected[i] = selected[i + 1];
            }
        }
    } else {
        for (int i = from; i > to; i--) {
            p->hand[i] = p->hand[i - 1];
            if (selected) {
                selected[i] = selected[i - 1];
            }
        }
    }

    p->hand[to] = temp;
    if (selected) {
        selected[to] = sel_temp;
    }
}

static void backup_hand(GuiGame *game, Player *p) {
    if (!game || !p) {
        return;
    }
    if (p->hand_count <= 0) {
        game->hand_backup_count = 0;
        return;
    }
    memcpy(game->hand_backup, p->hand, sizeof(Tile) * (size_t)p->hand_count);
    game->hand_backup_count = p->hand_count;
}

static void restore_hand(GuiGame *game, Player *p) {
    if (!game || !p) {
        return;
    }
    if (game->hand_backup_count <= 0) {
        p->hand_count = 0;
        return;
    }
    memcpy(p->hand, game->hand_backup, sizeof(Tile) * (size_t)game->hand_backup_count);
    p->hand_count = game->hand_backup_count;
}

static void reset_drag_state(GuiGame *game) {
    if (!game) {
        return;
    }
    game->drag_pending = false;
    game->dragging = false;
    game->drag_candidate_index = -1;
    game->drag_hand_index = -1;
    game->drag_source = DRAG_SRC_NONE;
    game->drag_candidate_table_comb = -1;
    game->drag_candidate_table_index = -1;
    game->drag_table_comb = -1;
    game->drag_table_index = -1;
    game->drag_w = 0.0f;
    game->drag_h = 0.0f;
}

static bool handle_game_over(GuiGame *game) {
    if (!game) {
        return false;
    }
    if (!is_game_over(game->players, game->num_players, &game->deck)) {
        return false;
    }

    calculate_final_scores(game->players, game->num_players);
    save_scores_to_file(game->players, game->num_players, "scores.txt");

    printf("[UI] Partie terminee. Scores sauvegardes dans scores.txt\n");

    if (game->players_initialized) {
        free_players(game->players, game->num_players);
        game->players_initialized = false;
    }
    free_table(&game->table);
    free_table(&game->table_backup);
    init_table(&game->table);
    init_table(&game->table_backup);

    game->state = GUI_STATE_MENU;
    ui_input_reset(game);
    return true;
}

static void begin_turn(GuiGame *game) {
    if (!game) {
        return;
    }
    Player *p = &game->players[game->current_player];
    backup_hand(game, p);
    game->turn_points = 0;
    game->turn_played = false;
    ui_clear_selection(game->selected, MAX_TILES);
    game->active_comb_index = -1;
    reset_drag_state(game);
}

static void end_turn(GuiGame *game) {
    if (!game) {
        return;
    }
    if (handle_game_over(game)) {
        return;
    }
    free_table(&game->table_backup);
    game->table_backup = clone_table(&game->table);
    game->current_player = (game->current_player + 1) % game->num_players;
    game->last_player = game->current_player;
    game->last_hand_count = game->players[game->current_player].hand_count;
    begin_turn(game);
}

static void ai_fill_index(Player *p, int tile_index[5][14]) {
    for (int c = 0; c < 5; c++) {
        for (int v = 0; v < 14; v++) {
            tile_index[c][v] = -1;
        }
    }
    for (int i = 0; i < p->hand_count; i++) {
        Tile *t = &p->hand[i];
        if (t->is_joker) {
            continue;
        }
        if (t->color >= 1 && t->color <= 4 && t->value >= 1 && t->value <= 13) {
            if (tile_index[t->color][t->value] == -1) {
                tile_index[t->color][t->value] = i;
            }
        }
    }
}

static bool ai_pick_group(Player *p, const int tile_index[5][14], Combinaison *out, int *out_points) {
    int best_points = 0;
    int best_val = 0;
    int best_indices[4] = {0};
    int best_count = 0;

    for (int val = 1; val <= 13; val++) {
        int indices[4];
        int count = 0;
        for (int color = 1; color <= 4; color++) {
            int idx = tile_index[color][val];
            if (idx >= 0) {
                indices[count++] = idx;
            }
        }
        if (count >= 3) {
            int use = count > 4 ? 4 : count;
            int points = val * use;
            if (points > best_points) {
                best_points = points;
                best_val = val;
                best_count = use;
                for (int i = 0; i < use; i++) {
                    best_indices[i] = indices[i];
                }
            }
        }
    }

    if (best_points <= 0 || best_count < 3) {
        return false;
    }

    out->count = best_count;
    out->type = IS_GROUPE;
    out->tiles = (Tile *)malloc(sizeof(Tile) * (size_t)best_count);
    for (int i = 0; i < best_count; i++) {
        out->tiles[i] = p->hand[best_indices[i]];
    }
    if (out_points) {
        *out_points = best_val * best_count;
    }
    return true;
}

static bool ai_pick_suite(Player *p, const int tile_index[5][14], Combinaison *out, int *out_points) {
    int best_len = 0;
    int best_color = 0;
    int best_start = 0;

    for (int color = 1; color <= 4; color++) {
        int v = 1;
        while (v <= 13) {
            if (tile_index[color][v] >= 0) {
                int start = v;
                while (v <= 13 && tile_index[color][v] >= 0) {
                    v++;
                }
                int len = v - start;
                if (len >= 3 && len > best_len) {
                    best_len = len;
                    best_color = color;
                    best_start = start;
                }
            } else {
                v++;
            }
        }
    }

    if (best_len < 3) {
        return false;
    }

    out->count = best_len;
    out->type = IS_SUITE;
    out->tiles = (Tile *)malloc(sizeof(Tile) * (size_t)best_len);
    int sum = 0;
    for (int i = 0; i < best_len; i++) {
        int val = best_start + i;
        int idx = tile_index[best_color][val];
        out->tiles[i] = p->hand[idx];
        sum += val;
    }
    if (out_points) {
        *out_points = sum;
    }
    return true;
}

static bool ai_pick_best_combination(Player *p, Combinaison *out, int *out_points) {
    int tile_index[5][14];
    ai_fill_index(p, tile_index);

    Combinaison group = {0};
    Combinaison suite = {0};
    int points_group = 0;
    int points_suite = 0;
    bool has_group = ai_pick_group(p, tile_index, &group, &points_group);
    bool has_suite = ai_pick_suite(p, tile_index, &suite, &points_suite);

    if (has_group && (!has_suite || points_group >= points_suite)) {
        *out = group;
        if (out_points) {
            *out_points = points_group;
        }
        if (suite.tiles) {
            free(suite.tiles);
        }
        return true;
    }

    if (has_suite) {
        *out = suite;
        if (out_points) {
            *out_points = points_suite;
        }
        if (group.tiles) {
            free(group.tiles);
        }
        return true;
    }

    if (group.tiles) {
        free(group.tiles);
    }
    if (suite.tiles) {
        free(suite.tiles);
    }
    return false;
}

static void ai_take_turn(GuiGame *game) {
    if (!game) {
        return;
    }
    Player *p = &game->players[game->current_player];
    if (!p->is_ai) {
        return;
    }

    Combinaison comb = {0};
    int points = 0;
    bool has_combo = ai_pick_best_combination(p, &comb, &points);

    if (!p->has_initial_meld) {
        if (!has_combo || points < 30) {
            if (game->deck.top > 0) {
                add_tile_to_player(p, draw_tile(&game->deck));
            }
            end_turn(game);
            if (comb.tiles) {
                free(comb.tiles);
            }
            return;
        }
        p->has_initial_meld = 1;
    }

    if (!has_combo) {
        if (game->deck.top > 0) {
            add_tile_to_player(p, draw_tile(&game->deck));
        }
        end_turn(game);
        return;
    }

    add_combinaison_to_table(&game->table, comb);
    for (int i = 0; i < comb.count; i++) {
        remove_tile_from_hand(p, comb.tiles[i].id);
    }
    free(comb.tiles);
    end_turn(game);
}

void ui_match_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h) {
    const float margin = 20.0f;
    const float sidebar_w = 220.0f;
    const float bottom_h = 180.0f;
    const float action_w = 170.0f;

    double now = gui_get_time_seconds();
    double dt = now - game->last_time;
    if (game->last_time <= 0.0) {
        dt = 0.0;
    }
    if (dt < 0.0 || dt > 1.0) {
        dt = 0.0;
    }
    game->last_time = now;

    if (game->notification_timer > 0.0f) {
        game->notification_timer -= (float)dt;
        if (game->notification_timer <= 0.0f) {
            game->notification_timer = 0.0f;
            game->notification[0] = '\0';
        }
    }

    Rect sidebar = rect_make(margin, margin, sidebar_w, fb_h - margin * 2.0f);
    Rect table_area = rect_make(margin + sidebar_w + margin,
                                margin,
                                fb_w - sidebar_w - margin * 3.0f,
                                fb_h - bottom_h - margin * 2.0f);
    Rect bottom_area = rect_make(margin,
                                 fb_h - bottom_h - margin,
                                 fb_w - margin * 2.0f,
                                 bottom_h);

    Rect menu_btn = rect_make(sidebar.x, sidebar.y, sidebar.w, 36.0f);

    Rect sort_panel = rect_make(bottom_area.x,
                                bottom_area.y + 10.0f,
                                sidebar_w,
                                bottom_area.h - 20.0f);
    Rect sort_color_btn = rect_make(sort_panel.x + 10.0f,
                                    sort_panel.y + 12.0f,
                                    sort_panel.w - 20.0f,
                                    40.0f);
    Rect sort_value_btn = rect_make(sort_panel.x + 10.0f,
                                    sort_panel.y + 62.0f,
                                    sort_panel.w - 20.0f,
                                    40.0f);

    Rect action_panel = rect_make(bottom_area.x + bottom_area.w - action_w,
                                  bottom_area.y + 10.0f,
                                  action_w - 10.0f,
                                  bottom_area.h - 20.0f);
    Rect play_btn = rect_make(action_panel.x + 10.0f,
                              action_panel.y + 10.0f,
                              action_panel.w - 20.0f,
                              40.0f);
    Rect validate_btn = rect_make(action_panel.x + 10.0f,
                                  action_panel.y + 58.0f,
                                  action_panel.w - 20.0f,
                                  40.0f);
    Rect draw_btn = rect_make(action_panel.x + 10.0f,
                              action_panel.y + 106.0f,
                              action_panel.w - 20.0f,
                              40.0f);

    Rect rack_area = rect_make(bottom_area.x + sidebar_w + margin,
                               bottom_area.y + 10.0f,
                               bottom_area.w - sidebar_w - action_w - margin * 2.0f,
                               bottom_area.h - 20.0f);

    double mx = 0.0, my = 0.0;
    gui_get_mouse_pos(w, &mx, &my);
    bool mouse_down = gui_mouse_button_down(w, GUI_MOUSE_LEFT);
    bool mouse_pressed = mouse_down && !game->prev_mouse_down;
    bool mouse_released = !mouse_down && game->prev_mouse_down;

    if (game->players[game->current_player].is_ai) {
        ai_take_turn(game);
        if (game->state != GUI_STATE_MATCH) {
            game->prev_mouse_down = mouse_down;
            return;
        }
    }

    r2d_fill_rect(sidebar.x, sidebar.y, sidebar.w, sidebar.h, 0.11f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(sidebar.x, sidebar.y, sidebar.w, sidebar.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    r2d_fill_rect(table_area.x, table_area.y, table_area.w, table_area.h, 0.16f, 0.18f, 0.20f, 1.0f);
    r2d_stroke_rect(table_area.x, table_area.y, table_area.w, table_area.h, 0.35f, 0.36f, 0.38f, 1.0f, 2.0f);

    r2d_fill_rect(bottom_area.x, bottom_area.y, bottom_area.w, bottom_area.h, 0.10f, 0.11f, 0.12f, 1.0f);
    r2d_stroke_rect(bottom_area.x, bottom_area.y, bottom_area.w, bottom_area.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    r2d_fill_rect(menu_btn.x, menu_btn.y, menu_btn.w, menu_btn.h, 0.20f, 0.21f, 0.22f, 1.0f);
    r2d_stroke_rect(menu_btn.x, menu_btn.y, menu_btn.w, menu_btn.h, 0.45f, 0.45f, 0.45f, 1.0f, 2.0f);
    ui_draw_text_centered(menu_btn, 1.6f, "MENU", 0.90f, 0.90f, 0.90f, 1.0f);

    float player_panel_y = menu_btn.y + menu_btn.h + 20.0f;
    float player_panel_h = 78.0f;
    float player_gap = 18.0f;
    for (int i = 0; i < game->num_players; i++) {
        Rect panel = rect_make(sidebar.x + 10.0f,
                               player_panel_y + i * (player_panel_h + player_gap),
                               sidebar.w - 20.0f,
                               player_panel_h);
        float base_r = (i == game->current_player) ? 0.18f : 0.10f;
        float base_g = (i == game->current_player) ? 0.20f : 0.12f;
        float base_b = (i == game->current_player) ? 0.24f : 0.14f;
        float stroke_r = (i == game->current_player) ? 0.80f : 0.55f;
        float stroke_g = (i == game->current_player) ? 0.80f : 0.55f;
        float stroke_b = (i == game->current_player) ? 0.65f : 0.55f;
        r2d_fill_rect(panel.x, panel.y, panel.w, panel.h, base_r, base_g, base_b, 1.0f);
        r2d_stroke_rect(panel.x, panel.y, panel.w, panel.h, stroke_r, stroke_g, stroke_b, 1.0f, 2.0f);
        ui_draw_text(panel.x + 10.0f, panel.y + 12.0f, 1.5f, game->players[i].name,
                     0.96f, 0.96f, 0.96f, 1.0f);
        char info[64];
        snprintf(info, sizeof(info), "Score: %d%s", game->players[i].score,
                 game->players[i].is_ai ? " (IA)" : "");
        ui_draw_text(panel.x + 10.0f, panel.y + 36.0f, 1.1f, info,
                     0.88f, 0.88f, 0.88f, 1.0f);
    }

    r2d_fill_rect(sort_panel.x, sort_panel.y, sort_panel.w, sort_panel.h, 0.12f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(sort_panel.x, sort_panel.y, sort_panel.w, sort_panel.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    bool hover_sort_color = point_in_rect((float)mx, (float)my, sort_color_btn);
    bool hover_sort_value = point_in_rect((float)mx, (float)my, sort_value_btn);

    float sortc_r = 0.18f, sortc_g = 0.20f, sortc_b = 0.22f;
    if (hover_sort_color) {
        sortc_r += 0.06f;
        sortc_g += 0.06f;
        sortc_b += 0.06f;
    }
    if (mouse_down && hover_sort_color) {
        sortc_r *= 0.8f;
        sortc_g *= 0.8f;
        sortc_b *= 0.8f;
    }
    r2d_fill_rect(sort_color_btn.x, sort_color_btn.y, sort_color_btn.w, sort_color_btn.h, sortc_r, sortc_g, sortc_b, 1.0f);
    r2d_stroke_rect(sort_color_btn.x, sort_color_btn.y, sort_color_btn.w, sort_color_btn.h,
                    hover_sort_color ? 0.75f : 0.55f,
                    hover_sort_color ? 0.75f : 0.55f,
                    hover_sort_color ? 0.75f : 0.55f,
                    1.0f,
                    2.0f);
    ui_draw_text_centered(sort_color_btn, 1.2f, "TRIER COULEUR", 0.92f, 0.92f, 0.92f, 1.0f);

    float sortv_r = 0.18f, sortv_g = 0.20f, sortv_b = 0.22f;
    if (hover_sort_value) {
        sortv_r += 0.06f;
        sortv_g += 0.06f;
        sortv_b += 0.06f;
    }
    if (mouse_down && hover_sort_value) {
        sortv_r *= 0.8f;
        sortv_g *= 0.8f;
        sortv_b *= 0.8f;
    }
    r2d_fill_rect(sort_value_btn.x, sort_value_btn.y, sort_value_btn.w, sort_value_btn.h, sortv_r, sortv_g, sortv_b, 1.0f);
    r2d_stroke_rect(sort_value_btn.x, sort_value_btn.y, sort_value_btn.w, sort_value_btn.h,
                    hover_sort_value ? 0.75f : 0.55f,
                    hover_sort_value ? 0.75f : 0.55f,
                    hover_sort_value ? 0.75f : 0.55f,
                    1.0f,
                    2.0f);
    ui_draw_text_centered(sort_value_btn, 1.2f, "TRIER VALEUR", 0.92f, 0.92f, 0.92f, 1.0f);

    r2d_fill_rect(action_panel.x, action_panel.y, action_panel.w, action_panel.h, 0.12f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(action_panel.x, action_panel.y, action_panel.w, action_panel.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    bool hover_play = point_in_rect((float)mx, (float)my, play_btn);
    bool hover_validate = point_in_rect((float)mx, (float)my, validate_btn);
    bool hover_draw = point_in_rect((float)mx, (float)my, draw_btn);

    float play_r = 0.20f, play_g = 0.30f, play_b = 0.20f;
    if (hover_play) {
        play_r += 0.05f;
        play_g += 0.05f;
        play_b += 0.05f;
    }
    if (mouse_down && hover_play) {
        play_r *= 0.8f;
        play_g *= 0.8f;
        play_b *= 0.8f;
    }
    r2d_fill_rect(play_btn.x, play_btn.y, play_btn.w, play_btn.h, play_r, play_g, play_b, 1.0f);
    r2d_stroke_rect(play_btn.x, play_btn.y, play_btn.w, play_btn.h, 0.60f, 0.80f, 0.60f, 1.0f, 2.0f);
    ui_draw_text_centered(play_btn, 1.4f, "JOUER", 0.95f, 0.95f, 0.95f, 1.0f);

    float val_r = 0.20f, val_g = 0.22f, val_b = 0.30f;
    if (hover_validate) {
        val_r += 0.05f;
        val_g += 0.05f;
        val_b += 0.05f;
    }
    if (mouse_down && hover_validate) {
        val_r *= 0.8f;
        val_g *= 0.8f;
        val_b *= 0.8f;
    }
    r2d_fill_rect(validate_btn.x, validate_btn.y, validate_btn.w, validate_btn.h, val_r, val_g, val_b, 1.0f);
    r2d_stroke_rect(validate_btn.x, validate_btn.y, validate_btn.w, validate_btn.h, 0.60f, 0.70f, 0.90f, 1.0f, 2.0f);
    ui_draw_text_centered(validate_btn, 1.4f, "VALIDER", 0.95f, 0.95f, 0.95f, 1.0f);

    float draw_r = 0.22f, draw_g = 0.20f, draw_b = 0.18f;
    if (hover_draw) {
        draw_r += 0.05f;
        draw_g += 0.05f;
        draw_b += 0.05f;
    }
    if (mouse_down && hover_draw) {
        draw_r *= 0.8f;
        draw_g *= 0.8f;
        draw_b *= 0.8f;
    }
    r2d_fill_rect(draw_btn.x, draw_btn.y, draw_btn.w, draw_btn.h, draw_r, draw_g, draw_b, 1.0f);
    r2d_stroke_rect(draw_btn.x, draw_btn.y, draw_btn.w, draw_btn.h, 0.70f, 0.60f, 0.50f, 1.0f, 2.0f);
    ui_draw_text_centered(draw_btn, 1.2f, "PIOCHER", 0.95f, 0.95f, 0.95f, 1.0f);

    int grid_cols = 20;
    int grid_rows = 8;
    float grid_pad = 8.0f;

    float cell_w = (table_area.w - grid_pad * (grid_cols + 1)) / (float)grid_cols;
    float cell_h = cell_w * 1.35f;
    float grid_h = grid_rows * cell_h + (grid_rows + 1) * grid_pad;
    if (grid_h > table_area.h) {
        cell_h = (table_area.h - grid_pad * (grid_rows + 1)) / (float)grid_rows;
        cell_w = cell_h / 1.35f;
    }

    float grid_w = grid_cols * cell_w + (grid_cols + 1) * grid_pad;
    grid_h = grid_rows * cell_h + (grid_rows + 1) * grid_pad;

    float grid_x = table_area.x + (table_area.w - grid_w) * 0.5f;
    float grid_y = table_area.y + (table_area.h - grid_h) * 0.5f;

    int total_cells = grid_cols * grid_rows;
    for (int i = 0; i < total_cells; i++) {
        int row = i / grid_cols;
        int col = i % grid_cols;
        float x = grid_x + grid_pad + col * (cell_w + grid_pad);
        float y = grid_y + grid_pad + row * (cell_h + grid_pad);
        Rect cell = rect_make(x, y, cell_w, cell_h);

        if (i == 0) {
            r2d_fill_rect(cell.x, cell.y, cell.w, cell.h, 0.12f, 0.16f, 0.18f, 1.0f);
        }
        r2d_stroke_rect(cell.x, cell.y, cell.w, cell.h, 0.30f, 0.30f, 0.30f, 1.0f, 1.5f);
    }

    TableTileHit table_hits[MAX_TILES];
    int table_hit_count = 0;
    int hover_table_comb = -1;
    int hover_table_hit = -1;
    int cell_index = 0;
    for (int ci = 0; ci < game->table.count && cell_index < total_cells; ci++) {
        Combinaison *c = &game->table.table[ci];
        for (int ti = 0; ti < c->count && cell_index < total_cells; ti++) {
            int row = cell_index / grid_cols;
            int col = cell_index % grid_cols;
            float x = grid_x + grid_pad + col * (cell_w + grid_pad);
            float y = grid_y + grid_pad + row * (cell_h + grid_pad);
            Rect cell = rect_make(x, y, cell_w, cell_h);

            bool hovered = point_in_rect((float)mx, (float)my, cell);
            if (hovered && hover_table_comb == -1) {
                hover_table_comb = ci;
                hover_table_hit = table_hit_count;
            }

            ui_draw_tile(cell, &c->tiles[ti], false, ci == game->active_comb_index, hovered, mouse_down && hovered);

            table_hits[table_hit_count].rect = cell;
            table_hits[table_hit_count].comb_index = ci;
            table_hits[table_hit_count].tile_index = ti;
            table_hit_count++;
            cell_index++;
        }
        cell_index++;
    }

    if (game->notification_timer > 0.0f && game->notification[0]) {
        float alpha = 1.0f;
        if (game->notification_timer < 0.4f) {
            alpha = game->notification_timer / 0.4f;
        }
        float nr = 0.18f, ng = 0.22f, nb = 0.30f;
        if (game->notification_kind == NOTICE_WARN) {
            nr = 0.55f; ng = 0.38f; nb = 0.18f;
        } else if (game->notification_kind == NOTICE_ERROR) {
            nr = 0.60f; ng = 0.18f; nb = 0.18f;
        }
        Rect notice = rect_make(table_area.x + 20.0f,
                                table_area.y + 12.0f,
                                table_area.w - 40.0f,
                                34.0f);
        r2d_fill_rect(notice.x, notice.y, notice.w, notice.h, nr, ng, nb, alpha);
        r2d_stroke_rect(notice.x, notice.y, notice.w, notice.h, 0.90f, 0.90f, 0.90f, alpha, 2.0f);
        ui_draw_text_centered(notice, 1.2f, game->notification, 0.98f, 0.98f, 0.98f, alpha);
    }

    Player *p = &game->players[game->current_player];

    int rack_slots = p->hand_count > 14 ? p->hand_count : 14;
    float rack_pad = 8.0f;
    float tile_h = rack_area.h - 16.0f;
    float tile_w = tile_h * 0.72f;
    float rack_needed = rack_slots * tile_w + (rack_slots - 1) * rack_pad;
    if (rack_needed > rack_area.w) {
        tile_w = (rack_area.w - (rack_slots - 1) * rack_pad) / (float)rack_slots;
        tile_h = tile_w / 0.72f;
    }

    float rack_start_x = rack_area.x + (rack_area.w - (rack_slots * tile_w + (rack_slots - 1) * rack_pad)) * 0.5f;
    float rack_start_y = rack_area.y + (rack_area.h - tile_h) * 0.5f;

    Rect hand_rects[MAX_TILES];
    int hand_rect_count = 0;

    for (int i = 0; i < rack_slots; i++) {
        float x = rack_start_x + i * (tile_w + rack_pad);
        Rect slot = rect_make(x, rack_start_y, tile_w, tile_h);
        r2d_stroke_rect(slot.x, slot.y, slot.w, slot.h, 0.25f, 0.25f, 0.25f, 1.0f, 1.5f);

        if (i < p->hand_count) {
            hand_rects[i] = slot;
            hand_rect_count++;
        }
    }

    int hover_hand_index = -1;
    for (int i = 0; i < hand_rect_count; i++) {
        if (point_in_rect((float)mx, (float)my, hand_rects[i])) {
            hover_hand_index = i;
            break;
        }
    }

    if (mouse_pressed) {
        if (hover_hand_index >= 0) {
            game->drag_pending = true;
            game->drag_source = DRAG_SRC_HAND;
            game->drag_candidate_index = hover_hand_index;
            game->drag_start_x = (float)mx;
            game->drag_start_y = (float)my;
        } else if (hover_table_hit >= 0) {
            game->drag_pending = true;
            game->drag_source = DRAG_SRC_TABLE;
            game->drag_candidate_table_comb = table_hits[hover_table_hit].comb_index;
            game->drag_candidate_table_index = table_hits[hover_table_hit].tile_index;
            game->drag_start_x = (float)mx;
            game->drag_start_y = (float)my;
            game->drag_offset_x = (float)mx - table_hits[hover_table_hit].rect.x;
            game->drag_offset_y = (float)my - table_hits[hover_table_hit].rect.y;
            game->drag_w = table_hits[hover_table_hit].rect.w;
            game->drag_h = table_hits[hover_table_hit].rect.h;
        }
    }

    if (game->drag_pending && mouse_down && !game->dragging) {
        float dx = (float)mx - game->drag_start_x;
        float dy = (float)my - game->drag_start_y;
        if (dx * dx + dy * dy > 36.0f) {
            game->dragging = true;
            game->drag_pending = false;
            if (game->drag_source == DRAG_SRC_HAND) {
                game->drag_hand_index = game->drag_candidate_index;
                if (game->drag_hand_index >= 0 && game->drag_hand_index < p->hand_count) {
                    game->drag_tile = p->hand[game->drag_hand_index];
                    game->drag_offset_x = (float)mx - hand_rects[game->drag_hand_index].x;
                    game->drag_offset_y = (float)my - hand_rects[game->drag_hand_index].y;
                    game->drag_w = tile_w;
                    game->drag_h = tile_h;
                }
            } else if (game->drag_source == DRAG_SRC_TABLE) {
                game->drag_table_comb = game->drag_candidate_table_comb;
                game->drag_table_index = game->drag_candidate_table_index;
                if (game->drag_table_comb >= 0 && game->drag_table_comb < game->table.count) {
                    Combinaison *c = &game->table.table[game->drag_table_comb];
                    if (c->tiles && game->drag_table_index >= 0 && game->drag_table_index < c->count) {
                        game->drag_tile = c->tiles[game->drag_table_index];
                    }
                }
            }
        }
    }

    bool hand_dragging = game->dragging && game->drag_source == DRAG_SRC_HAND;
    int drag_target = -1;
    if (hand_dragging) {
        if (hover_hand_index >= 0) {
            drag_target = hover_hand_index;
        } else if (point_in_rect((float)mx, (float)my, rack_area)) {
            float rel = (float)mx - rack_start_x;
            int idx = (int)(rel / (tile_w + rack_pad) + 0.5f);
            drag_target = clamp_int(idx, 0, p->hand_count - 1);
        }
    }

    int slot_to_tile[MAX_TILES];
    int tile_count = p->hand_count;
    if (!hand_dragging || tile_count <= 0) {
        for (int i = 0; i < tile_count; i++) {
            slot_to_tile[i] = i;
        }
    } else {
        int write = 0;
        for (int i = 0; i < tile_count; i++) {
            if (i == game->drag_hand_index) {
                continue;
            }
            slot_to_tile[write++] = i;
        }
        int insert = drag_target >= 0 ? drag_target : game->drag_hand_index;
        insert = clamp_int(insert, 0, write);
        for (int i = write; i > insert; i--) {
            slot_to_tile[i] = slot_to_tile[i - 1];
        }
        slot_to_tile[insert] = -1;
    }

    for (int i = 0; i < hand_rect_count; i++) {
        Rect slot = hand_rects[i];
        int tile_index = slot_to_tile[i];
        if (tile_index < 0) {
            r2d_fill_rect(slot.x, slot.y, slot.w, slot.h, 0.18f, 0.18f, 0.18f, 0.45f);
            r2d_stroke_rect(slot.x, slot.y, slot.w, slot.h, 0.35f, 0.35f, 0.35f, 1.0f, 2.0f);
            continue;
        }

        bool hovered = (!hand_dragging) && hover_hand_index == tile_index;
        bool pressed = mouse_down && hovered;
        if (game->drag_pending && game->drag_source == DRAG_SRC_HAND && tile_index == game->drag_candidate_index) {
            pressed = true;
        }
        ui_draw_tile(slot, &p->hand[tile_index], game->selected[tile_index], false, hovered, pressed);
    }

    if (game->dragging) {
        Rect drag_rect = rect_make((float)mx - game->drag_offset_x,
                                   (float)my - game->drag_offset_y,
                                   game->drag_w,
                                   game->drag_h);
        ui_draw_tile(drag_rect, &game->drag_tile, false, false, true, true);
    }

    if (game->last_player != game->current_player || game->last_hand_count != p->hand_count) {
        ui_clear_selection(game->selected, MAX_TILES);
        game->last_player = game->current_player;
        game->last_hand_count = p->hand_count;
    }

    if (mouse_released) {
        if (game->dragging) {
            if (game->drag_source == DRAG_SRC_HAND) {
                int drop_comb = -1;
                if (hover_table_comb >= 0) {
                    drop_comb = hover_table_comb;
                } else if (game->active_comb_index >= 0 && point_in_rect((float)mx, (float)my, table_area)) {
                    drop_comb = game->active_comb_index;
                }

                if (drop_comb >= 0) {
                    Tile t = game->drag_tile;
                    if (!can_edit_table(game, p)) {
                        ui_notify(game, NOTICE_ERROR, "Premiere pose: 30 points requis.");
                        audio_play_sfx(AUDIO_SFX_INVALID);
                    } else {
                        int insert_idx = game->table.table[drop_comb].count;
                        if (hover_table_hit >= 0 && table_hits[hover_table_hit].comb_index == drop_comb) {
                            Rect hrect = table_hits[hover_table_hit].rect;
                            int base = table_hits[hover_table_hit].tile_index;
                            if ((float)mx > hrect.x + hrect.w * 0.5f) {
                                base++;
                            }
                            insert_idx = base;
                        }
                        int before_count = game->table.table[drop_comb].count;
                        table_insert_tile(&game->table, drop_comb, insert_idx, t);
                        if (game->table.table[drop_comb].count == before_count) {
                            ui_notify(game, NOTICE_ERROR, "Ajout impossible : memoire insuffisante.");
                            audio_play_sfx(AUDIO_SFX_INVALID);
                        } else {
                            remove_tile_from_hand(p, t.id);
                            game->active_comb_index = drop_comb;
                            ui_clear_selection(game->selected, MAX_TILES);
                            game->last_hand_count = p->hand_count;
                            game->turn_played = true;
                            if (!is_valid_combination(&game->table.table[drop_comb])) {
                                ui_notify(game, NOTICE_WARN, "Combinaison invalide: corrigez avant de valider.");
                                audio_play_sfx(AUDIO_SFX_INVALID);
                            } else {
                                audio_play_sfx(AUDIO_SFX_PLAY);
                            }
                        }
                    }
                } else if (point_in_rect((float)mx, (float)my, rack_area)) {
                    int drop_index = drag_target >= 0 ? drag_target : game->drag_hand_index;
                    hand_move_tile(p, game->selected, game->drag_hand_index, drop_index);
                }
            } else if (game->drag_source == DRAG_SRC_TABLE) {
                if (!can_edit_table(game, p)) {
                    ui_notify(game, NOTICE_ERROR, "Premiere pose: 30 points requis.");
                    audio_play_sfx(AUDIO_SFX_INVALID);
                } else {
                    int target_comb = -1;
                    int insert_idx = -1;

                    if (hover_table_comb >= 0) {
                        target_comb = hover_table_comb;
                        insert_idx = game->table.table[target_comb].count;
                        if (hover_table_hit >= 0 && table_hits[hover_table_hit].comb_index == target_comb) {
                            Rect hrect = table_hits[hover_table_hit].rect;
                            int base = table_hits[hover_table_hit].tile_index;
                            if ((float)mx > hrect.x + hrect.w * 0.5f) {
                                base++;
                            }
                            insert_idx = base;
                        }
                    } else if (point_in_rect((float)mx, (float)my, table_area)) {
                        if (game->table.count < MAX_COMB) {
                            int origin_comb = game->drag_table_comb;
                            int origin_idx = game->drag_table_index;
                            Tile t = game->drag_tile;
                            int prev_count = game->table.count;
                            table_remove_tile(&game->table, origin_comb, origin_idx);
                            int new_idx = table_add_new_comb(&game->table, t);
                            if (new_idx >= 0) {
                                game->active_comb_index = new_idx;
                                game->turn_played = true;
                                ui_notify(game, NOTICE_WARN, "Nouvelle combinaison a completer.");
                                audio_play_sfx(AUDIO_SFX_PLAY);
                            } else {
                                if (game->table.count < prev_count) {
                                    if (origin_comb > game->table.count) {
                                        origin_comb = game->table.count;
                                    }
                                }
                                if (origin_comb >= 0 && origin_comb < game->table.count) {
                                    table_insert_tile(&game->table, origin_comb, origin_idx, t);
                                } else {
                                    table_add_new_comb(&game->table, t);
                                }
                            }
                        } else {
                            ui_notify(game, NOTICE_ERROR, "Limite de combinaisons atteinte.");
                            audio_play_sfx(AUDIO_SFX_INVALID);
                        }
                    } else if (point_in_rect((float)mx, (float)my, rack_area)) {
                        ui_notify(game, NOTICE_WARN, "Impossible de reprendre une tuile sur le support.");
                        audio_play_sfx(AUDIO_SFX_INVALID);
                    }

                    if (target_comb >= 0) {
                        bool same_slot = (target_comb == game->drag_table_comb) &&
                                         (insert_idx == game->drag_table_index ||
                                          insert_idx == game->drag_table_index + 1);
                        if (!same_slot) {
                            int final_target = target_comb;
                            if (game->drag_table_comb >= 0 &&
                                game->drag_table_comb < game->table.count &&
                                game->table.table[game->drag_table_comb].count == 1 &&
                                game->drag_table_comb < target_comb) {
                                final_target--;
                            }
                            table_move_tile(&game->table,
                                            game->drag_table_comb,
                                            game->drag_table_index,
                                            target_comb,
                                            insert_idx);
                            game->active_comb_index = final_target;
                            game->turn_played = true;
                            if (!is_valid_combination(&game->table.table[final_target])) {
                                ui_notify(game, NOTICE_WARN, "Combinaison invalide: corrigez avant de valider.");
                                audio_play_sfx(AUDIO_SFX_INVALID);
                            } else {
                                audio_play_sfx(AUDIO_SFX_PLAY);
                            }
                        } else {
                            game->active_comb_index = target_comb;
                        }
                    }
                }
            }

            reset_drag_state(game);
        } else if (game->drag_pending) {
            if (game->drag_source == DRAG_SRC_HAND) {
                int idx = game->drag_candidate_index;
                if (idx >= 0 && idx < p->hand_count) {
                    game->selected[idx] = !game->selected[idx];
                }
            } else if (game->drag_source == DRAG_SRC_TABLE) {
                if (game->drag_candidate_table_comb >= 0 && game->drag_candidate_table_comb < game->table.count) {
                    game->active_comb_index = game->drag_candidate_table_comb;
                }
            }
            reset_drag_state(game);
        }
    }

    bool input_blocked = game->drag_pending || game->dragging;

    if (mouse_pressed && !input_blocked) {
        if (point_in_rect((float)mx, (float)my, menu_btn)) {
            audio_play_sfx(AUDIO_SFX_CLICK);
            game->state = GUI_STATE_MENU;
            game->menu_selected_name = 0;
            ui_input_reset(game);
            return;
        }

        if (point_in_rect((float)mx, (float)my, sort_color_btn)) {
            audio_play_sfx(AUDIO_SFX_CLICK);
            sort_player_hand(p, 1);
            ui_clear_selection(game->selected, MAX_TILES);
            if (!game->turn_played) {
                backup_hand(game, p);
            }
        } else if (point_in_rect((float)mx, (float)my, sort_value_btn)) {
            audio_play_sfx(AUDIO_SFX_CLICK);
            sort_player_hand(p, 0);
            ui_clear_selection(game->selected, MAX_TILES);
            if (!game->turn_played) {
                backup_hand(game, p);
            }
        } else if (point_in_rect((float)mx, (float)my, draw_btn)) {
            if (game->turn_played) {
                ui_notify(game, NOTICE_WARN, "Pioche impossible apres avoir joue.");
                audio_play_sfx(AUDIO_SFX_INVALID);
            } else {
                if (game->deck.top > 0) {
                    add_tile_to_player(p, draw_tile(&game->deck));
                    audio_play_sfx(AUDIO_SFX_DRAW);
                }
                end_turn(game);
                game->prev_mouse_down = mouse_down;
                return;
            }
        } else if (point_in_rect((float)mx, (float)my, play_btn)) {
            audio_play_sfx(AUDIO_SFX_CLICK);
            int sel_idx[MAX_TILES];
            int sel_count = 0;
            for (int i = 0; i < p->hand_count; i++) {
                if (game->selected[i]) {
                    sel_idx[sel_count++] = i;
                }
            }

            if (sel_count >= 3) {
                Combinaison comb;
                comb.tiles = (Tile *)malloc(sizeof(Tile) * (size_t)sel_count);
                comb.count = sel_count;
                comb.type = 0;

                for (int i = 0; i < sel_count; i++) {
                    comb.tiles[i] = p->hand[sel_idx[i]];
                }

                int is_group = is_valid_group(&comb);
                int is_suite = is_valid_suite(&comb);
                if (is_group || is_suite) {
                    comb.type = is_suite ? IS_SUITE : IS_GROUPE;
                    int points = combinaison_points(&comb);
                    add_combinaison_to_table(&game->table, comb);
                    for (int i = 0; i < comb.count; i++) {
                        remove_tile_from_hand(p, comb.tiles[i].id);
                    }
                    game->active_comb_index = game->table.count - 1;
                    game->last_hand_count = p->hand_count;
                    game->turn_played = true;
                    if (!p->has_initial_meld) {
                        game->turn_points += points;
                    }
                    audio_play_sfx(AUDIO_SFX_PLAY);
                } else {
                    ui_notify(game, NOTICE_ERROR, "Combinaison invalide (>=3 requis).");
                    audio_play_sfx(AUDIO_SFX_INVALID);
                }
                free(comb.tiles);
                ui_clear_selection(game->selected, MAX_TILES);
            } else if (sel_count == 1) {
                if (!p->has_initial_meld && game->turn_points < 30) {
                    ui_notify(game, NOTICE_ERROR, "Premiere pose: 30 points requis.");
                    audio_play_sfx(AUDIO_SFX_INVALID);
                } else if (game->active_comb_index >= 0) {
                    Tile t = p->hand[sel_idx[0]];
                    if (add_tile_to_table_comb(&game->table, game->active_comb_index, t)) {
                        remove_tile_from_hand(p, t.id);
                        game->last_hand_count = p->hand_count;
                        game->turn_played = true;
                        audio_play_sfx(AUDIO_SFX_PLAY);
                    } else {
                        ui_notify(game, NOTICE_WARN, "Ajout impossible : combinaison invalide.");
                        audio_play_sfx(AUDIO_SFX_INVALID);
                    }
                } else {
                    ui_notify(game, NOTICE_WARN, "Aucun groupe actif a completer.");
                    audio_play_sfx(AUDIO_SFX_INVALID);
                }
                ui_clear_selection(game->selected, MAX_TILES);
            } else {
                ui_notify(game, NOTICE_WARN, "Selection vide ou trop courte.");
                audio_play_sfx(AUDIO_SFX_INVALID);
            }
        } else if (point_in_rect((float)mx, (float)my, validate_btn)) {
            if (!game->turn_played) {
                if (game->deck.top > 0) {
                    add_tile_to_player(p, draw_tile(&game->deck));
                    audio_play_sfx(AUDIO_SFX_DRAW);
                }
                end_turn(game);
                game->prev_mouse_down = mouse_down;
                return;
            }

            if (!p->has_initial_meld && game->turn_points < 30) {
                ui_notify(game, NOTICE_ERROR, "Premiere pose: 30 points minimum.");
                audio_play_sfx(AUDIO_SFX_INVALID);
                free_table(&game->table);
                game->table = clone_table(&game->table_backup);
                restore_hand(game, p);
                ui_clear_selection(game->selected, MAX_TILES);
                game->active_comb_index = -1;
                game->turn_points = 0;
                game->turn_played = false;
                game->last_hand_count = p->hand_count;
                if (game->deck.top > 0) {
                    add_tile_to_player(p, draw_tile(&game->deck));
                    audio_play_sfx(AUDIO_SFX_DRAW);
                }
                end_turn(game);
                game->prev_mouse_down = mouse_down;
                return;
            }

            if (!verify_whole_table(&game->table)) {
                ui_notify(game, NOTICE_ERROR, "Table invalide, retour a l'etat precedent.");
                audio_play_sfx(AUDIO_SFX_INVALID);
                free_table(&game->table);
                game->table = clone_table(&game->table_backup);
                restore_hand(game, p);
                game->active_comb_index = -1;
                ui_clear_selection(game->selected, MAX_TILES);
                game->turn_points = 0;
                game->turn_played = false;
                game->last_hand_count = p->hand_count;
            } else {
                if (!p->has_initial_meld && game->turn_points >= 30) {
                    p->has_initial_meld = 1;
                }
                audio_play_sfx(AUDIO_SFX_VALIDATE);
                end_turn(game);
                game->prev_mouse_down = mouse_down;
                return;
            }
        } else {
            for (int i = 0; i < table_hit_count; i++) {
                if (point_in_rect((float)mx, (float)my, table_hits[i].rect)) {
                    game->active_comb_index = table_hits[i].comb_index;
                    break;
                }
            }
        }
    }

    game->prev_mouse_down = mouse_down;
}
