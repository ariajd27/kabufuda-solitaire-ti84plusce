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

#ifndef variables_include_file
#define variables_include_file

#define WINS_VAR_NAME "KBFDWINS"
#define SAVE_VAR_NAME "KBFDSAVE"

#define BKGND_COLOR 2
#define BORDER_COLOR 1
#define TOP_BORDER 25

#define FRAME_TIME 3277
#define HOLD_TIME 2

#define NUM_FREECELLS 4
#define NUM_TABLSLOTS 8
#define TABL_STACK_SIZE 8
#define LOCK_ICON_VOFFSET 17
#define LOCK_ICON_HOFFSET 9
#define FC_VPOS 50
#define TABL_VPOS 100
#define FC_HPOS 102
#define TABL_HPOS 42

#define TEXT_CHAR_WIDTH 8
#define SELCARD_DISP_X 5
#define SELCARD_DISP_Y 9
#define NUMWINS_DISP_X (GFX_LCD_WIDTH - 5 - 3 * TEXT_CHAR_WIDTH)

extern unsigned char tableau[NUM_TABLSLOTS][TABL_STACK_SIZE];
extern unsigned char freeCells[NUM_FREECELLS];
extern unsigned char fcUnlocked;

extern unsigned char cursorStack;
extern unsigned char cursorIndex;
extern enum cursorMode_t { SELECT, DROP } cursorMode;
extern unsigned char selectedCard;
extern unsigned char selectedQty;
extern unsigned char orgStack;
extern unsigned char orgIndex;

extern unsigned char progress;
extern unsigned char numWins;

#endif
