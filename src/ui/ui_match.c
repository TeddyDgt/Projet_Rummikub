#include "ui_match.h"

#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"

#include "Combinaisons.h"
#include "Players.h"
#include "Table.h"

#include "ui_input.h"
#include "ui_rect.h"
#include "ui_text.h"
#include "ui_tiles.h"
#include "ui_util.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    Rect rect;
    int comb_index;
} TableTileHit;

static int clamp_int(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
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

void ui_match_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h) {
    const float margin = 20.0f;
    const float sidebar_w = 220.0f;
    const float bottom_h = 180.0f;
    const float action_w = 170.0f;

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
                              action_panel.y + 12.0f,
                              action_panel.w - 20.0f,
                              48.0f);
    Rect validate_btn = rect_make(action_panel.x + 10.0f,
                                  action_panel.y + 70.0f,
                                  action_panel.w - 20.0f,
                                  48.0f);

    Rect rack_area = rect_make(bottom_area.x + sidebar_w + margin,
                               bottom_area.y + 10.0f,
                               bottom_area.w - sidebar_w - action_w - margin * 2.0f,
                               bottom_area.h - 20.0f);

    double mx = 0.0, my = 0.0;
    gui_get_mouse_pos(w, &mx, &my);
    bool mouse_down = gui_mouse_button_down(w, GUI_MOUSE_LEFT);
    bool mouse_pressed = mouse_down && !game->prev_mouse_down;
    bool mouse_released = !mouse_down && game->prev_mouse_down;

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
        float highlight = (i == game->current_player) ? 0.22f : 0.16f;
        r2d_fill_rect(panel.x, panel.y, panel.w, panel.h, highlight, highlight + 0.02f, highlight + 0.03f, 1.0f);
        r2d_stroke_rect(panel.x, panel.y, panel.w, panel.h, 0.40f, 0.40f, 0.40f, 1.0f, 2.0f);
        ui_draw_text(panel.x + 10.0f, panel.y + 12.0f, 1.5f, game->players[i].name,
                     0.92f, 0.92f, 0.92f, 1.0f);
    }

    r2d_fill_rect(sort_panel.x, sort_panel.y, sort_panel.w, sort_panel.h, 0.12f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(sort_panel.x, sort_panel.y, sort_panel.w, sort_panel.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    r2d_fill_rect(sort_color_btn.x, sort_color_btn.y, sort_color_btn.w, sort_color_btn.h, 0.18f, 0.20f, 0.22f, 1.0f);
    r2d_stroke_rect(sort_color_btn.x, sort_color_btn.y, sort_color_btn.w, sort_color_btn.h, 0.55f, 0.55f, 0.55f, 1.0f, 2.0f);
    ui_draw_text_centered(sort_color_btn, 1.2f, "TRIER COULEUR", 0.92f, 0.92f, 0.92f, 1.0f);

    r2d_fill_rect(sort_value_btn.x, sort_value_btn.y, sort_value_btn.w, sort_value_btn.h, 0.18f, 0.20f, 0.22f, 1.0f);
    r2d_stroke_rect(sort_value_btn.x, sort_value_btn.y, sort_value_btn.w, sort_value_btn.h, 0.55f, 0.55f, 0.55f, 1.0f, 2.0f);
    ui_draw_text_centered(sort_value_btn, 1.2f, "TRIER VALEUR", 0.92f, 0.92f, 0.92f, 1.0f);

    r2d_fill_rect(action_panel.x, action_panel.y, action_panel.w, action_panel.h, 0.12f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(action_panel.x, action_panel.y, action_panel.w, action_panel.h, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    r2d_fill_rect(play_btn.x, play_btn.y, play_btn.w, play_btn.h, 0.20f, 0.30f, 0.20f, 1.0f);
    r2d_stroke_rect(play_btn.x, play_btn.y, play_btn.w, play_btn.h, 0.60f, 0.80f, 0.60f, 1.0f, 2.0f);
    ui_draw_text_centered(play_btn, 1.6f, "JOUER", 0.95f, 0.95f, 0.95f, 1.0f);

    r2d_fill_rect(validate_btn.x, validate_btn.y, validate_btn.w, validate_btn.h, 0.20f, 0.22f, 0.30f, 1.0f);
    r2d_stroke_rect(validate_btn.x, validate_btn.y, validate_btn.w, validate_btn.h, 0.60f, 0.70f, 0.90f, 1.0f, 2.0f);
    ui_draw_text_centered(validate_btn, 1.6f, "VALIDER", 0.95f, 0.95f, 0.95f, 1.0f);

    int grid_cols = 18;
    int grid_rows = 4;
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
            }

            ui_draw_tile(cell, &c->tiles[ti], false, ci == game->active_comb_index, hovered, mouse_down && hovered);

            table_hits[table_hit_count].rect = cell;
            table_hits[table_hit_count].comb_index = ci;
            table_hit_count++;
            cell_index++;
        }
        cell_index++;
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

    if (mouse_pressed && hover_hand_index >= 0) {
        game->drag_pending = true;
        game->drag_candidate_index = hover_hand_index;
        game->drag_start_x = (float)mx;
        game->drag_start_y = (float)my;
    }

    if (game->drag_pending && mouse_down && !game->dragging) {
        float dx = (float)mx - game->drag_start_x;
        float dy = (float)my - game->drag_start_y;
        if (dx * dx + dy * dy > 36.0f) {
            game->dragging = true;
            game->drag_pending = false;
            game->drag_hand_index = game->drag_candidate_index;
            game->drag_tile = p->hand[game->drag_hand_index];
            game->drag_offset_x = (float)mx - hand_rects[game->drag_hand_index].x;
            game->drag_offset_y = (float)my - hand_rects[game->drag_hand_index].y;
        }
    }

    int drag_target = -1;
    if (game->dragging) {
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
    if (!game->dragging || tile_count <= 0) {
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

        bool hovered = !game->dragging && hover_hand_index == tile_index;
        bool pressed = mouse_down && hovered;
        if (game->drag_pending && tile_index == game->drag_candidate_index) {
            pressed = true;
        }
        ui_draw_tile(slot, &p->hand[tile_index], game->selected[tile_index], false, hovered, pressed);
    }

    if (game->dragging) {
        Rect drag_rect = rect_make((float)mx - game->drag_offset_x,
                                   (float)my - game->drag_offset_y,
                                   tile_w,
                                   tile_h);
        ui_draw_tile(drag_rect, &game->drag_tile, false, false, true, true);
    }

    if (game->last_player != game->current_player || game->last_hand_count != p->hand_count) {
        ui_clear_selection(game->selected, MAX_TILES);
        game->last_player = game->current_player;
        game->last_hand_count = p->hand_count;
    }

    if (mouse_released) {
        if (game->dragging) {
            int drop_comb = -1;
            if (hover_table_comb >= 0) {
                drop_comb = hover_table_comb;
            } else if (game->active_comb_index >= 0 && point_in_rect((float)mx, (float)my, table_area)) {
                drop_comb = game->active_comb_index;
            }

            if (drop_comb >= 0) {
                Tile t = game->drag_tile;
                if (add_tile_to_table_comb(&game->table, drop_comb, t)) {
                    remove_tile_from_hand(p, t.id);
                    game->active_comb_index = drop_comb;
                    ui_clear_selection(game->selected, MAX_TILES);
                    game->last_hand_count = p->hand_count;
                } else {
                    printf("[UI] Ajout impossible : combinaison invalide.\n");
                }
            } else if (point_in_rect((float)mx, (float)my, rack_area)) {
                int drop_index = drag_target >= 0 ? drag_target : game->drag_hand_index;
                hand_move_tile(p, game->selected, game->drag_hand_index, drop_index);
            }

            game->dragging = false;
            game->drag_pending = false;
            game->drag_candidate_index = -1;
            game->drag_hand_index = -1;
        } else if (game->drag_pending) {
            int idx = game->drag_candidate_index;
            if (idx >= 0 && idx < p->hand_count) {
                game->selected[idx] = !game->selected[idx];
            }
            game->drag_pending = false;
            game->drag_candidate_index = -1;
        }
    }

    bool input_blocked = game->drag_pending || game->dragging;

    if (mouse_pressed && !input_blocked) {
        if (point_in_rect((float)mx, (float)my, menu_btn)) {
            game->state = GUI_STATE_MENU;
            game->menu_selected_name = 0;
            ui_input_reset(game);
            return;
        }

        if (point_in_rect((float)mx, (float)my, sort_color_btn)) {
            sort_player_hand(p, 1);
            ui_clear_selection(game->selected, MAX_TILES);
        } else if (point_in_rect((float)mx, (float)my, sort_value_btn)) {
            sort_player_hand(p, 0);
            ui_clear_selection(game->selected, MAX_TILES);
        } else if (point_in_rect((float)mx, (float)my, play_btn)) {
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
                    add_combinaison_to_table(&game->table, comb);
                    for (int i = 0; i < comb.count; i++) {
                        remove_tile_from_hand(p, comb.tiles[i].id);
                    }
                    game->active_comb_index = game->table.count - 1;
                    game->last_hand_count = p->hand_count;
                } else {
                    printf("[UI] Combinaison invalide (>=3 requis).\n");
                }
                free(comb.tiles);
                ui_clear_selection(game->selected, MAX_TILES);
            } else if (sel_count == 1) {
                if (game->active_comb_index >= 0) {
                    Tile t = p->hand[sel_idx[0]];
                    if (add_tile_to_table_comb(&game->table, game->active_comb_index, t)) {
                        remove_tile_from_hand(p, t.id);
                        game->last_hand_count = p->hand_count;
                    } else {
                        printf("[UI] Ajout impossible : combinaison invalide.\n");
                    }
                } else {
                    printf("[UI] Aucun groupe actif a completer.\n");
                }
                ui_clear_selection(game->selected, MAX_TILES);
            } else {
                printf("[UI] Selection vide ou trop courte.\n");
            }
        } else if (point_in_rect((float)mx, (float)my, validate_btn)) {
            if (!verify_whole_table(&game->table)) {
                printf("[UI] Table invalide, retour a l'etat precedent.\n");
                free_table(&game->table);
                game->table = clone_table(&game->table_backup);
                game->active_comb_index = -1;
                ui_clear_selection(game->selected, MAX_TILES);
            } else {
                free_table(&game->table_backup);
                game->table_backup = clone_table(&game->table);
                game->current_player = (game->current_player + 1) % game->num_players;
                game->active_comb_index = -1;
                ui_clear_selection(game->selected, MAX_TILES);
                game->last_player = game->current_player;
                game->last_hand_count = game->players[game->current_player].hand_count;
            }
        } else {
            int hit = 0;
            for (int i = 0; i < table_hit_count; i++) {
                if (point_in_rect((float)mx, (float)my, table_hits[i].rect)) {
                    game->active_comb_index = table_hits[i].comb_index;
                    hit = 1;
                    break;
                }
            }

        }
    }

    game->prev_mouse_down = mouse_down;
}
