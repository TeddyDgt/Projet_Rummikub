#include "ui_tiles.h"

#include "gfx/renderer2d.h"
#include "ui_text.h"

#include <stdio.h>

/**
 * @brief Function tile_base_color.
 *
 * @param t Parameter t.
 * @param r Parameter r.
 * @param g Parameter g.
 * @param b Parameter b.
 * @return None.
 */
/**
 * @brief Function tile_base_color.
 *
 * @param t Parameter t.
 * @param r Parameter r.
 * @param g Parameter g.
 * @param b Parameter b.
 * @return None.
 */
static void tile_base_color(const Tile *t, float *r, float *g, float *b) {
    if (!t || t->is_joker) {
        *r = 0.90f;
        *g = 0.90f;
        *b = 0.90f;
        return;
    }

    switch (t->color) {
        case NOIR:
            *r = 0.20f;
            *g = 0.20f;
            *b = 0.20f;
            break;
        case ROUGE:
            *r = 0.80f;
            *g = 0.25f;
            *b = 0.25f;
            break;
        case BLEU:
            *r = 0.25f;
            *g = 0.45f;
            *b = 0.85f;
            break;
        case JAUNE:
            *r = 0.85f;
            *g = 0.75f;
            *b = 0.25f;
            break;
        default:
            *r = 0.75f;
            *g = 0.75f;
            *b = 0.75f;
            break;
    }
}

/**
 * @brief Function tile_text_color.
 *
 * @param t Parameter t.
 * @param r Parameter r.
 * @param g Parameter g.
 * @param b Parameter b.
 * @return None.
 */
/**
 * @brief Function tile_text_color.
 *
 * @param t Parameter t.
 * @param r Parameter r.
 * @param g Parameter g.
 * @param b Parameter b.
 * @return None.
 */
static void tile_text_color(const Tile *t, float *r, float *g, float *b) {
    float br = 0.85f, bg = 0.85f, bb = 0.85f;
    tile_base_color(t, &br, &bg, &bb);
    float lum = br * 0.2126f + bg * 0.7152f + bb * 0.0722f;
    if (lum < 0.45f) {
        *r = 0.95f;
        *g = 0.95f;
        *b = 0.95f;
    } else {
        *r = 0.10f;
        *g = 0.10f;
        *b = 0.10f;
    }
}

/**
 * @brief Function ui_draw_tile.
 *
 * @param r Parameter r.
 * @param t Parameter t.
 * @param selected Parameter selected.
 * @param highlight Parameter highlight.
 * @param hovered Parameter hovered.
 * @param pressed Parameter pressed.
 * @return None.
 */
/**
 * @brief Function ui_draw_tile.
 *
 * @param r Parameter r.
 * @param t Parameter t.
 * @param selected Parameter selected.
 * @param highlight Parameter highlight.
 * @param hovered Parameter hovered.
 * @param pressed Parameter pressed.
 * @return None.
 */
void ui_draw_tile(Rect r, const Tile *t, bool selected, bool highlight, bool hovered, bool pressed) {
    float tr = 0.85f, tg = 0.85f, tb = 0.85f;
    tile_base_color(t, &tr, &tg, &tb);

    r2d_fill_rect(r.x, r.y, r.w, r.h, tr, tg, tb, 1.0f);

    if (highlight) {
        r2d_stroke_rect(r.x - 2.0f, r.y - 2.0f, r.w + 4.0f, r.h + 4.0f,
                        0.95f, 0.80f, 0.20f, 1.0f, 2.0f);
    }

    if (selected) {
        r2d_stroke_rect(r.x, r.y, r.w, r.h, 0.20f, 0.90f, 0.40f, 1.0f, 3.0f);
    } else {
        r2d_stroke_rect(r.x, r.y, r.w, r.h, 0.15f, 0.15f, 0.15f, 1.0f, 2.0f);
    }

    if (t) {
        char label[8];
        if (t->is_joker) {
            snprintf(label, sizeof(label), "J");
        } else {
            snprintf(label, sizeof(label), "%d", t->value);
        }

        float scale = (r.h * 0.45f) / 7.0f;
        if (scale < 1.0f) {
            scale = 1.0f;
        }

        float tw = ui_text_width(label, scale);
        float th = ui_text_height(scale);
        float tx = r.x + (r.w - tw) * 0.5f;
        float ty = r.y + (r.h - th) * 0.5f;

        float cr = 0.0f, cg = 0.0f, cb = 0.0f;
        tile_text_color(t, &cr, &cg, &cb);
        ui_draw_text(tx, ty, scale, label, cr, cg, cb, 1.0f);
    }

    if (hovered) {
        r2d_stroke_rect(r.x - 1.0f, r.y - 1.0f, r.w + 2.0f, r.h + 2.0f,
                        0.90f, 0.90f, 0.90f, 1.0f, 1.5f);
    }

    if (pressed) {
        r2d_fill_rect(r.x, r.y, r.w, r.h, 0.0f, 0.0f, 0.0f, 0.25f);
    }
}


