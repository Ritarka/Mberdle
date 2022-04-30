#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "globals.h"
 

// The bottom of the screen => y=127
// A macro to reverse the coordinate for convenience (optional).
#define REVERSE_Y(x) (SIZE_Y-(x))

/**
 * Draws a sprite
 */
void bright_star();
void dim_star();
void small_star();
void clear(int, int, int, int);
void draw_lower_status();

 
#endif // GRAPHICS_H
