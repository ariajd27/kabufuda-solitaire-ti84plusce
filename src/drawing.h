#ifndef drawing_include_file
#define drawing_include_file

#include <graphx.h>
#include <gfx/gfx.h>

#define CARD_HEIGHT card1_height
#define CARD_WIDTH card1_width
#define CARD_SPACING 5
#define CARD_VOFFSET 8

extern gfx_sprite_t* cardSprite[11];

void drawBackground();
void drawCursor(unsigned char X, unsigned char Y);
void drawFrame();

void animateDeal();

#endif
