// Kabufuda Solitaire / KBFDSLTR for the TI-84 Plus CE
// Copyright (C) 2024 euphory
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
