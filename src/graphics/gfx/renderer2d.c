#include "renderer2d.h"

#include "ui/ui_text.h"
#include "ui/ui_tiles.h"
#include "Players.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

void r2d_begin(int fb_w, int fb_h) {
    glViewport(0, 0, fb_w, fb_h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)fb_w, (double)fb_h, 0.0, -1.0, 1.0); /* y vers le bas */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void r2d_fill_rect(float x, float y, float w, float h,
                   float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x,     y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x,     y + h);
    glEnd();
}

void r2d_stroke_rect(float x, float y, float w, float h,
                     float r, float g, float b, float a,
                     float thickness) {
    glLineWidth(thickness);
    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x,     y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x,     y + h);
    glEnd();
    glLineWidth(1.0f);
}

static void draw_rect_button(Rect r,
                             const char *label,
                             float base_r, float base_g, float base_b,
                             float hover_delta, float press_mul,
                             float stroke_r, float stroke_g, float stroke_b,
                             float stroke_hover_delta,
                             float text_scale,
                             float text_r, float text_g, float text_b,
                             bool hovered, bool pressed) {
    float r_col = base_r;
    float g_col = base_g;
    float b_col = base_b;
    if (hovered) {
        r_col += hover_delta;
        g_col += hover_delta;
        b_col += hover_delta;
    }
    if (pressed) {
        r_col *= press_mul;
        g_col *= press_mul;
        b_col *= press_mul;
    }

    float s_r = stroke_r + (hovered ? stroke_hover_delta : 0.0f);
    float s_g = stroke_g + (hovered ? stroke_hover_delta : 0.0f);
    float s_b = stroke_b + (hovered ? stroke_hover_delta : 0.0f);

    r2d_fill_rect(r.x, r.y, r.w, r.h, r_col, g_col, b_col, 1.0f);
    r2d_stroke_rect(r.x, r.y, r.w, r.h, s_r, s_g, s_b, 1.0f, 2.0f);
    ui_draw_text_centered(r, text_scale, label, text_r, text_g, text_b, 1.0f);
}

void draw_match_background(const MatchLayout *layout) {
    if (!layout) {
        return;
    }
    r2d_fill_rect(layout->sidebar.x, layout->sidebar.y, layout->sidebar.w, layout->sidebar.h,
                  0.11f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(layout->sidebar.x, layout->sidebar.y, layout->sidebar.w, layout->sidebar.h,
                    0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    r2d_fill_rect(layout->table_area.x, layout->table_area.y, layout->table_area.w, layout->table_area.h,
                  0.16f, 0.18f, 0.20f, 1.0f);
    r2d_stroke_rect(layout->table_area.x, layout->table_area.y, layout->table_area.w, layout->table_area.h,
                    0.35f, 0.36f, 0.38f, 1.0f, 2.0f);

    r2d_fill_rect(layout->bottom_area.x, layout->bottom_area.y, layout->bottom_area.w, layout->bottom_area.h,
                  0.10f, 0.11f, 0.12f, 1.0f);
    r2d_stroke_rect(layout->bottom_area.x, layout->bottom_area.y, layout->bottom_area.w, layout->bottom_area.h,
                    0.30f, 0.30f, 0.30f, 1.0f, 2.0f);
}

void draw_menu_button(const Rect *menu_btn) {
    if (!menu_btn) {
        return;
    }
    r2d_fill_rect(menu_btn->x, menu_btn->y, menu_btn->w, menu_btn->h,
                  0.20f, 0.21f, 0.22f, 1.0f);
    r2d_stroke_rect(menu_btn->x, menu_btn->y, menu_btn->w, menu_btn->h,
                    0.45f, 0.45f, 0.45f, 1.0f, 2.0f);
    ui_draw_text_centered(*menu_btn, 1.6f, "MENU", 0.90f, 0.90f, 0.90f, 1.0f);
}

void draw_player_list(const MatchLayout *layout, const GuiGame *game) {
    if (!layout || !game) {
        return;
    }
    float player_panel_y = layout->menu_btn.y + layout->menu_btn.h + 20.0f;
    float player_panel_h = 78.0f;
    float player_gap = 18.0f;
    for (int i = 0; i < game->num_players; i++) {
        Rect panel = rect_make(layout->sidebar.x + 10.0f,
                               player_panel_y + i * (player_panel_h + player_gap),
                               layout->sidebar.w - 20.0f,
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
        if (i == game->current_player) {
            int display_score = -calculate_hand_penalty((Player *)&game->players[i]);
            snprintf(info, sizeof(info), "Score: %d%s", display_score,
                     game->players[i].is_ai ? " (IA)" : "");
        } else {
            snprintf(info, sizeof(info), "Score: ?%s",
                     game->players[i].is_ai ? " (IA)" : "");
        }
        ui_draw_text(panel.x + 10.0f, panel.y + 36.0f, 1.1f, info,
                     0.88f, 0.88f, 0.88f, 1.0f);
    }
}

void draw_sort_buttons(const MatchLayout *layout, bool hover_color, bool hover_value, bool mouse_down) {
    if (!layout) {
        return;
    }
    r2d_fill_rect(layout->sort_panel.x, layout->sort_panel.y, layout->sort_panel.w, layout->sort_panel.h,
                  0.12f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(layout->sort_panel.x, layout->sort_panel.y, layout->sort_panel.w, layout->sort_panel.h,
                    0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    draw_rect_button(layout->sort_color_btn,
                     "TRIER COULEUR",
                     0.18f, 0.20f, 0.22f,
                     0.06f, 0.8f,
                     0.55f, 0.55f, 0.55f,
                     0.20f,
                     1.2f,
                     0.92f, 0.92f, 0.92f,
                     hover_color, mouse_down && hover_color);

    draw_rect_button(layout->sort_value_btn,
                     "TRIER VALEUR",
                     0.18f, 0.20f, 0.22f,
                     0.06f, 0.8f,
                     0.55f, 0.55f, 0.55f,
                     0.20f,
                     1.2f,
                     0.92f, 0.92f, 0.92f,
                     hover_value, mouse_down && hover_value);
}

void draw_action_buttons(const MatchLayout *layout, bool hover_play, bool hover_validate, bool hover_draw, bool mouse_down) {
    if (!layout) {
        return;
    }
    r2d_fill_rect(layout->action_panel.x, layout->action_panel.y, layout->action_panel.w, layout->action_panel.h,
                  0.12f, 0.12f, 0.13f, 1.0f);
    r2d_stroke_rect(layout->action_panel.x, layout->action_panel.y, layout->action_panel.w, layout->action_panel.h,
                    0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

    draw_rect_button(layout->play_btn,
                     "JOUER",
                     0.20f, 0.30f, 0.20f,
                     0.05f, 0.8f,
                     0.60f, 0.80f, 0.60f,
                     0.0f,
                     1.4f,
                     0.95f, 0.95f, 0.95f,
                     hover_play, mouse_down && hover_play);

    draw_rect_button(layout->validate_btn,
                     "VALIDER",
                     0.20f, 0.22f, 0.30f,
                     0.05f, 0.8f,
                     0.60f, 0.70f, 0.90f,
                     0.0f,
                     1.4f,
                     0.95f, 0.95f, 0.95f,
                     hover_validate, mouse_down && hover_validate);

    draw_rect_button(layout->draw_btn,
                     "PIOCHER",
                     0.22f, 0.20f, 0.18f,
                     0.05f, 0.8f,
                     0.70f, 0.60f, 0.50f,
                     0.0f,
                     1.2f,
                     0.95f, 0.95f, 0.95f,
                     hover_draw, mouse_down && hover_draw);
}

void draw_table_grid(const Rect *table_area,
                     int grid_cols,
                     int grid_rows,
                     float grid_pad,
                     float cell_w,
                     float cell_h,
                     float grid_x,
                     float grid_y,
                     int total_cells) {
    if (!table_area) {
        return;
    }
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
}

void draw_notification(const Rect *table_area, const GuiGame *game) {
    if (!table_area || !game) {
        return;
    }
    if (game->notification_timer <= 0.0f || !game->notification[0]) {
        return;
    }
    float alpha = 1.0f;
    if (game->notification_timer < 0.4f) {
        alpha = game->notification_timer / 0.4f;
    }
    float nr = 0.18f, ng = 0.22f, nb = 0.30f;
    if (game->notification_kind == 1) {
        nr = 0.55f; ng = 0.38f; nb = 0.18f;
    } else if (game->notification_kind == 2) {
        nr = 0.60f; ng = 0.18f; nb = 0.18f;
    }
    Rect notice = rect_make(table_area->x + 20.0f,
                            table_area->y + 12.0f,
                            table_area->w - 40.0f,
                            34.0f);
    r2d_fill_rect(notice.x, notice.y, notice.w, notice.h, nr, ng, nb, alpha);
    r2d_stroke_rect(notice.x, notice.y, notice.w, notice.h, 0.90f, 0.90f, 0.90f, alpha, 2.0f);
    ui_draw_text_centered(notice, 1.2f, game->notification, 0.98f, 0.98f, 0.98f, alpha);
}
