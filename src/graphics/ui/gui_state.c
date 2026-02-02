#include "gui_screen.h"

#include "audio.h"
#include "Players.h"
#include "Table.h"
#include "ui_persist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Initialise l'état global du moteur graphique et du jeu.
 * - Initialise le générateur de nombres aléatoires.
 * - Charge la persistance des joueurs (noms, types IA).
 * - Initialise la table de jeu et ses sauvegardes.
 * - Configure les timers de notification.
 * @param game Pointeur vers la structure GuiGame à initialiser.
 */
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

    game->notification[0] = '\0';
    game->notification_timer = 0.0f;
    game->notification_kind = 0;
    game->last_time = 0.0;

    audio_init();
    audio_play_bgm();
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

    audio_shutdown();
}
