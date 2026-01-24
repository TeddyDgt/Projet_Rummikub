#include "gui_screen.h"

#include "Players.h"
#include "Table.h"
#include "ui_persist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void gui_game_init(GuiGame *game) {
    if (!game) {
        return;
    }

    memset(game, 0, sizeof(*game));
    srand((unsigned int)time(NULL));

    game->state = GUI_STATE_MENU;
    game->menu_player_count = 2;
    for (int i = 0; i < 4; i++) {
        snprintf(game->menu_player_names[i], sizeof(game->menu_player_names[i]),
                 "Joueur %d", i + 1);
        game->menu_is_ai[i] = false;
    }
    ui_persist_load_players(game->menu_player_names, game->menu_is_ai, &game->menu_player_count);
    game->menu_selected_name = 0;
    game->players_initialized = false;
    init_table(&game->table);
    init_table(&game->table_backup);
}

void gui_game_shutdown(GuiGame *game) {
    if (!game) {
        return;
    }

    if (game->players_initialized) {
        free_players(game->players, game->num_players);
        game->players_initialized = false;
    }
    free_table(&game->table);
    free_table(&game->table_backup);
}
