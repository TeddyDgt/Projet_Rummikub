#ifndef RENDERER2D_H
#define RENDERER2D_H

/* Rendu UI match */
#include <stdbool.h>
#include "ui/ui_rect.h"
#include "ui/gui_screen.h"

typedef struct {
    Rect sidebar;
    Rect table_area;
    Rect bottom_area;
    Rect menu_btn;
    Rect sort_panel;
    Rect sort_color_btn;
    Rect sort_value_btn;
    Rect action_panel;
    Rect play_btn;
    Rect validate_btn;
    Rect draw_btn;
    Rect rack_area;
} MatchLayout;

/* Initialise une projection 2D en coordonnÃ©es pixels
   (0,0) en haut-gauche, (w,h) en bas-droite */
void r2d_begin(int fb_w, int fb_h);

/* Rectangle rempli */
void r2d_fill_rect(float x, float y, float w, float h,
                   float r, float g, float b, float a);

/* Contour rectangle */
void r2d_stroke_rect(float x, float y, float w, float h,
                     float r, float g, float b, float a,
                     float thickness);

void draw_match_background(const MatchLayout *layout);
void draw_menu_button(const Rect *menu_btn);
void draw_player_list(const MatchLayout *layout, const GuiGame *game);
void draw_sort_buttons(const MatchLayout *layout, bool hover_color, bool hover_value, bool mouse_down);
void draw_action_buttons(const MatchLayout *layout, bool hover_play, bool hover_validate, bool hover_draw, bool mouse_down);
void draw_table_grid(const Rect *table_area,
                     int grid_cols,
                     int grid_rows,
                     float grid_pad,
                     float cell_w,
                     float cell_h,
                     float grid_x,
                     float grid_y,
                     int total_cells);
void draw_notification(const Rect *table_area, const GuiGame *game);

#endif
