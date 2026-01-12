#include "platform/gui_platform.h"
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

        gui_swap_buffers(w);
    }

    gui_destroy_window(w);
    return 0;
}
