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

#endif
