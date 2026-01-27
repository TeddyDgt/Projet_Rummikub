#include "ui_input.h"

#include "platform/gui_platform.h"

#include <string.h>

bool ui_key_pressed(GuiGame *game, GuiWindow *w, int key) {
    if (!game || !w) {
        return false;
    }
    if (key < 0 || key >= 512) {
        return false;
    }
    bool down = gui_key_down(w, key);
    bool pressed = down && !game->key_prev[key];
    game->key_prev[key] = down;
    return pressed;
}

void ui_input_reset(GuiGame *game) {
    if (!game) {
        return;
    }
    game->prev_mouse_down = false;
    memset(game->key_prev, 0, sizeof(game->key_prev));
}
