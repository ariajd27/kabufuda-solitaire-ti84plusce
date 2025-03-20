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

#ifndef variables_include_file
#define variables_include_file

#define WINS_VAR_NAME "KBFDWINS"
#define SAVE_VAR_NAME "KBFDSAVE"

#define FRAME_TIME 3277
#define HOLD_TIME 2

#define NUM_FREECELLS 4
#define NUM_TABLSLOTS 4
#define TABL_STACK_SIZE 16

#define PROGRESS_COMPLETE 4
#define DECK_ORG 0xff

#define CARD_RED 0x20
#define CARD_SUIT 0x30
#define CARD_NUMBER 0x0f
#define CARD_KING 12
#define CARD_EMPTY 0x00
#define CARD_EXISTS 0x80

typedef unsigned char card_t;

extern card_t tableau[NUM_TABLSLOTS][TABL_STACK_SIZE];
extern card_t freeCells[NUM_FREECELLS];

extern unsigned char cursorStack;
extern unsigned char cursorIndex;
extern enum cursorMode_t { SELECT, DROP } cursorMode;
extern card_t selectedCard;
extern unsigned char orgStack;
extern unsigned char orgIndex;

extern unsigned char progress;
extern unsigned char numWins;

extern unsigned char deck[7];
extern unsigned char deckCards;

#endif
