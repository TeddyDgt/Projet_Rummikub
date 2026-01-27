#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"
#include "ui/gui_screen.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/gl.h>

int main(void) {
    GuiWindow *w = gui_create_window(1280, 720, "Rummikub - GUI");
    if (!w) {
        return 1;
    }

    GuiGame game;
    gui_game_init(&game);

    while (!gui_should_close(w)) {
        gui_poll_events();

        int fw = 0, fh = 0;
        gui_get_framebuffer_size(w, &fw, &fh);
        glViewport(0, 0, fw, fh);

        glClearColor(0.08f, 0.09f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        r2d_begin(fw, fh);
        gui_game_render(&game, w, fw, fh);

        gui_swap_buffers(w);
    }

    gui_game_shutdown(&game);

    gui_destroy_window(w);
    return 0;
}
