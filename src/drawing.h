// Calculation Solitaire / CALCSLTR for the TI-84 Plus CE
// Copyright (C) 2025 euphory
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef drawing_include_file
#define drawing_include_file

#include <graphx.h>
#include <gfx/gfx.h>
#include "variables.h"

#define CARD_HEIGHT 41
#define CARD_WIDTH 27
#define CARD_SPACING 5
#define CARD_VOFFSET 7

#define CARD_NUMERAL_HOFFSET 1
#define CARD_NUMERAL_VOFFSET 1
#define CARD_FSUIT_HOFFSET 1
#define CARD_FSUIT_VOFFSET 7
#define LOCK_ICON_VOFFSET 17
#define LOCK_ICON_HOFFSET 9
#define FC_VPOS 50
#define TABL_VPOS 100
#define FC_HPOS 98
#define TABL_HPOS 98

#define TEXT_CHAR_WIDTH 8
#define SELCARD_DISP_X 5
#define SELCARD_DISP_Y 9
#define NUMWINS_DISP_X (GFX_LCD_WIDTH - 5 - 3 * TEXT_CHAR_WIDTH)

#define BKGND_COLOR 3
#define BORDER_COLOR 0
#define CARD_COLOR 0
#define BLACK_COLOR 1
#define RED_COLOR 2

#define TOP_BORDER 25

#define DECK_VPOS FC_VPOS
#define DECK_HPOS (FC_HPOS - CARD_WIDTH - CARD_SPACING)
#define DECK_CARDS_PER_HEIGHT 9

#define SELCARD_XPOS 147
#define SELCARD_YPOS 210

#define MOVE_ANIM_LENGTH 8000

extern gfx_sprite_t* cardSprite[11];

unsigned int getCursorX();
unsigned char getCursorY();
void drawCursor();
void drawCard(card_t toDraw, unsigned int x, unsigned char y);
void drawStack(unsigned char stackIndex);
void drawBar();
void drawFrame(bool drawSelected);
void animateMove(unsigned int x0, unsigned char y0, unsigned int x1, unsigned char y1);

#define animateGrab() animateMove(getCursorX(), getCursorY(), SELCARD_XPOS, SELCARD_YPOS)
#define animateDrop() animateMove(SELCARD_XPOS, SELCARD_YPOS, getCursorX(), getCursorY())
#define animateDraw() animateMove(DECK_HPOS, DECK_VPOS, SELCARD_XPOS, SELCARD_YPOS)

#endif
