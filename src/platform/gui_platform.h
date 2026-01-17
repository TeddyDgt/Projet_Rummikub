#ifndef GUI_PLATFORM_H
#define GUI_PLATFORM_H

#include <stdbool.h>

typedef struct GuiWindow GuiWindow;

GuiWindow* gui_create_window(int width, int height, const char* title);
void gui_destroy_window(GuiWindow* w);

bool gui_should_close(GuiWindow* w);
void gui_poll_events(void);
void gui_swap_buffers(GuiWindow* w);

void gui_get_framebuffer_size(GuiWindow* w, int* out_w, int* out_h);

typedef enum {
    GUI_MOUSE_LEFT = 0,
    GUI_MOUSE_RIGHT = 1,
    GUI_MOUSE_MIDDLE = 2
} GuiMouseButton;

void gui_get_mouse_pos(GuiWindow* w, double* out_x, double* out_y);
bool gui_mouse_button_down(GuiWindow* w, int button);
bool gui_key_down(GuiWindow* w, int key);

#endif
