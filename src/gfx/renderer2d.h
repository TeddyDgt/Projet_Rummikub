#ifndef RENDERER2D_H
#define RENDERER2D_H

/* Initialise une projection 2D en coordonnées pixels
   (0,0) en haut-gauche, (w,h) en bas-droite */
void r2d_begin(int fb_w, int fb_h);

/* Rectangle rempli */
void r2d_fill_rect(float x, float y, float w, float h,
                   float r, float g, float b, float a);

/* Contour rectangle */
void r2d_stroke_rect(float x, float y, float w, float h,
                     float r, float g, float b, float a,
                     float thickness);

#endif
