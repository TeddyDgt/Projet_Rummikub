#ifndef UI_UTIL_H
#define UI_UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static inline void ui_clear_selection(bool *selection, int max_count) {
    if (!selection || max_count <= 0) {
        return;
    }
    memset(selection, 0, sizeof(bool) * (size_t)max_count);
}

#endif
