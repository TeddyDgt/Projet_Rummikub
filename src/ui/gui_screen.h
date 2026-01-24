#ifndef GUI_SCREEN_H
#define GUI_SCREEN_H

#include <stdbool.h>

#include "structs.h"

typedef struct GuiWindow GuiWindow;

typedef enum {
    GUI_STATE_MENU = 0,
    GUI_STATE_MATCH = 1
} GuiState;

typedef struct {
    Deck deck;
    Table table;
    Table table_backup;
    Player players[4];
    int num_players;
    int current_player;
    bool selected[MAX_TILES];
    int active_comb_index;
    int last_hand_count;
    int last_player;
    Tile hand_backup[MAX_TILES];
    int hand_backup_count;
    int turn_points;
    bool turn_played;
    bool prev_mouse_down;
    bool key_prev[512];
    bool drag_pending;
    bool dragging;
    int drag_candidate_index;
    int drag_hand_index;
    float drag_start_x;
    float drag_start_y;
    float drag_offset_x;
    float drag_offset_y;
    Tile drag_tile;
    bool players_initialized;
    int menu_player_count;
    char menu_player_names[4][16];
    bool menu_is_ai[4];
    int menu_selected_name;
    GuiState state;
} GuiGame;

void gui_game_init(GuiGame *game);
void gui_game_shutdown(GuiGame *game);
void gui_game_render(GuiGame *game, GuiWindow *w, int fb_w, int fb_h);

#endif
