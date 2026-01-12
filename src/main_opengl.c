#include "platform/gui_platform.h"
#include "gfx/renderer2d.h"

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>


// boucle minimale: clear + ESC quitte (j'effectue un test de la fenêtre OpenGL !)
int main(void) {
    GuiWindow* w = gui_create_window(1280, 720, "Rummikub - GUI (Module 1)");
    if (!w) return 1;

    while (!gui_should_close(w)) {
        gui_poll_events();

        int fw = 0, fh = 0;
        gui_get_framebuffer_size(w, &fw, &fh);
        glViewport(0, 0, fw, fh);

        glClearColor(0.1f, 0.12f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        r2d_begin(fw, fh);

        /* Zone table (centre) */
        r2d_fill_rect(60, 60, fw - 120, fh - 220, 0.18f, 0.20f, 0.22f, 1.0f);
        r2d_stroke_rect(60, 60, fw - 120, fh - 220, 0.35f, 0.36f, 0.38f, 1.0f, 2.0f);

        /* Zone chevalet (bas) */
        r2d_fill_rect(60, fh - 140, fw - 120, 80, 0.12f, 0.13f, 0.14f, 1.0f);
        r2d_stroke_rect(60, fh - 140, fw - 120, 80, 0.30f, 0.30f, 0.30f, 1.0f, 2.0f);

        /* “Tuiles” (placeholders) sur le chevalet */
        float tile_w = 46.0f, tile_h = 64.0f;
        float x = 80.0f;
        float y = (float)fh - 132.0f;

        for (int i = 0; i < 14; i++) {
            r2d_fill_rect(x, y, tile_w, tile_h, 0.92f, 0.92f, 0.92f, 1.0f);
            r2d_stroke_rect(x, y, tile_w, tile_h, 0.15f, 0.15f, 0.15f, 1.0f, 2.0f);
            x += tile_w + 8.0f;
        }


        gui_swap_buffers(w);
    }

    gui_destroy_window(w);
    return 0;
}
