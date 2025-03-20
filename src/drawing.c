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

#include "drawing.h"

#include <graphx.h>
#include <time.h>
#include "gfx/gfx.h"
#include "variables.h"

#define DEAL_ANIM_DECK_X ((GFX_LCD_WIDTH - CARD_WIDTH) / 2)
#define DEAL_ANIM_DECK_Y (GFX_LCD_HEIGHT - CARD_HEIGHT * 2 / 3)
#define DEAL_ANIM_DECK_FRAME_TIME 100
#define DEAL_ANIM_DECK_PAUSE_TIME 1000
#define DEAL_ANIM_TOTAL_TIME 100000
#define DEAL_ANIM_TRAVEL_TIME 6000
#define DEAL_ANIM_FRAME_TIME 250
#define DEAL_ANIM_LAUNCH_INTERVAL ((DEAL_ANIM_TOTAL_TIME - DEAL_ANIM_TRAVEL_TIME) / 40)

void drawBackground()
{
	gfx_FillScreen(BKGND_COLOR);
	gfx_SetColor(BORDER_COLOR);
	gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, TOP_BORDER);
}

void drawCursor(unsigned char X, unsigned char Y)
{
	if (cursorMode == SELECT) gfx_TransparentSprite(selcorner, X - 2, Y - 2);
	else gfx_TransparentSprite(drpcorner, X - 2, Y - 2);
}

void drawMask(unsigned char *data, unsigned char rows, unsigned char x, unsigned char y)
{
	unsigned char yy = y;

	for (unsigned char row = 0; row < rows; row++)
	{
		unsigned char row_data = data[row];

		for (unsigned char xx = x; xx < x + 8; xx++)
		{
			if (row_data & 0x80)
			{
				gfx_SetPixel(xx, yy);
			}

			row_data <<= 1;
		}

		yy++;
	}
}

void drawCard(card_t toDraw, unsigned char x, unsigned char y)
{
	if (!(toDraw & CARD_EXISTS)) return;

	gfx_SetColor(CARD_COLOR);
	gfx_FillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT);

	gfx_SetColor((toDraw & CARD_RED) ? RED_COLOR : BLACK_COLOR);

	drawMask(numerals_tiles_data[toDraw & CARD_NUMBER], 5, x + CARD_NUMERAL_HOFFSET, y + CARD_NUMERAL_VOFFSET);
	drawMask(small_suits_tiles_data[(toDraw & CARD_SUIT) >> 4], 4, x + CARD_FSUIT_HOFFSET, y + CARD_FSUIT_VOFFSET);
}

void drawDeck()
{
	unsigned char y = DECK_VPOS;
	for (unsigned char x = 0; x <= deckCards; x += DECK_CARDS_PER_HEIGHT)
	{
		gfx_Sprite(card_back, DECK_HPOS, y);
		y -= 2;
	}
}

void drawFrame()
{
	drawBackground();

	drawDeck();

	for (unsigned char i = 0; i < NUM_FREECELLS; i++)
	{
		unsigned char cardX = FC_HPOS + i * (CARD_WIDTH + CARD_SPACING);

		drawCard(freeCells[i], cardX, FC_VPOS);

		if (i == cursorStack) drawCursor(cardX, FC_VPOS);
	}

	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
	{
		for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
		{
			unsigned char cardX = TABL_HPOS + i * (CARD_WIDTH + CARD_SPACING);
			unsigned char cardY = TABL_VPOS + j * CARD_VOFFSET;

			if (progress < PROGRESS_COMPLETE && i + NUM_FREECELLS == cursorStack && j == cursorIndex) drawCursor(cardX, cardY);
			
			drawCard(tableau[i][j], cardX, cardY);
		}
	}

	if (progress < PROGRESS_COMPLETE)
	{
		gfx_SetTextXY(SELCARD_DISP_X, SELCARD_DISP_Y);
		if (selectedCard & CARD_EXISTS)
		{
			gfx_PrintUInt((selectedCard & CARD_NUMBER) + 1, 1);
			gfx_PrintString("OF ");

			switch ((selectedCard & CARD_SUIT) >> 4)
			{
				case 0:
					gfx_PrintString("SPADES");
					break;
				case 1:
					gfx_PrintString("CLUBS");
					break;
				case 2:
					gfx_PrintString("HEARTS");
					break;
				case 3:
					gfx_PrintString("DIAMONDS");
					break;
			}

			drawCard(selectedCard, SELCARD_DISP_X, SELCARD_DISP_Y);
		}
		else gfx_PrintString("EMPTY");

		if (cursorMode == SELECT) gfx_PrintStringXY("SELECT", GFX_LCD_WIDTH / 2 - 3 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);
		else gfx_PrintStringXY("DROP", GFX_LCD_WIDTH / 2 - 2 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);
	}

	else gfx_PrintStringXY("COMPLETE", GFX_LCD_WIDTH / 2 - 4 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);

	gfx_SetTextXY(NUMWINS_DISP_X, SELCARD_DISP_Y);
	gfx_PrintUInt(numWins, 3);
	
	gfx_BlitBuffer();
}