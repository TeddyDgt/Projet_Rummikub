#include "renderer2d.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

void r2d_begin(int fb_w, int fb_h) {
    glViewport(0, 0, fb_w, fb_h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)fb_w, (double)fb_h, 0.0, -1.0, 1.0); /* y vers le bas */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void r2d_fill_rect(float x, float y, float w, float h,
                   float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x,     y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x,     y + h);
    glEnd();
}

void r2d_stroke_rect(float x, float y, float w, float h,
                     float r, float g, float b, float a,
                     float thickness) {
    glLineWidth(thickness);
    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x,     y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x,     y + h);
    glEnd();
    glLineWidth(1.0f);
}
