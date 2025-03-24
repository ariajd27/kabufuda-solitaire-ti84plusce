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

// these are used for placing pips on cards
const unsigned char segments[] = {
	0x01, 0x24, 0x25, 0x48, 0x49, 0x4a, 0xa8, 0x90, 0x91, 0xb4
};
const unsigned char pipCode[] = {
	0x04, 07, 02, 15, 02, 07, 12, 15, 12,
	0x02, 07, 07, 15, 07,
	0x01, 11, 07,
	0x84, 12, 29, 20, 29, 12, 39, 20, 39,
	0x82, 12, 34, 20, 34,
	0x81, 16, 34,
	0x02, 07, 17, 15, 17,
	0x01, 11, 17
};

void drawMask(const unsigned char *data, unsigned char rows, unsigned int x, unsigned char y)
{
	unsigned char yy = y;

	for (unsigned char row = 0; row < rows; row++)
	{
		unsigned char row_data = data[row];

		for (unsigned int xx = x; xx < x + 8; xx++)
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

void drawMaskInverted(const unsigned char *data, unsigned char rows, unsigned int x, unsigned char y)
{
	unsigned char yy = y - rows;

	for (unsigned char row = 1; row <= rows; row++)
	{
		unsigned char row_data = data[rows - row];

		for (unsigned int xx = x - 8; xx < x; xx++)
		{
			if (row_data & 0x01)
			{
				gfx_SetPixel(xx, yy);
			}

			row_data >>= 1;
		}

		yy++;
	}
}

unsigned int getCursorX()
{
	if (cursorStack < NUM_FREECELLS) return FC_HPOS + cursorStack * (CARD_WIDTH + CARD_SPACING);
	else return TABL_HPOS + (cursorStack - NUM_FREECELLS) * (CARD_WIDTH + CARD_SPACING);
}

unsigned char getCursorY()
{
	if (cursorStack < NUM_FREECELLS) return FC_VPOS;
	else return TABL_VPOS + cursorIndex * CARD_VOFFSET;
}

void drawCursor()
{
	const unsigned int X = getCursorX();
	const unsigned char Y = getCursorY();

	gfx_SetColor(cursorMode == SELECT ? BLACK_COLOR : RED_COLOR);

	drawMask(selcorner_tile_0_data, 6, X - 2, Y - 2);
	drawMask(selcorner_tile_1_data, 6, X + CARD_WIDTH - 4, Y - 2);
	drawMaskInverted(selcorner_tile_1_data, 6, X + 4, Y + CARD_HEIGHT + 2);
	drawMaskInverted(selcorner_tile_0_data, 6, X + CARD_WIDTH + 2, Y + CARD_HEIGHT + 2);
}

#include <debug.h>

void drawCard(card_t toDraw, unsigned int x, unsigned char y, bool useCutoff)
{
	if (!(toDraw & CARD_EXISTS)) return;

	gfx_SetColor(CARD_COLOR);
	gfx_FillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT);

	gfx_SetColor((toDraw & CARD_RED) ? RED_COLOR : BLACK_COLOR);

	const unsigned char cardNumber = toDraw & CARD_NUMBER;
	const unsigned char cardSuit = (toDraw & CARD_SUIT) >> 4;

	drawMask(numerals_tiles_data[cardNumber], 5, x + CARD_NUMERAL_HOFFSET, y + CARD_NUMERAL_VOFFSET);
	drawMask(small_suits_tiles_data[cardSuit], 4, x + CARD_FSUIT_HOFFSET, y + CARD_FSUIT_VOFFSET);

	if (!useCutoff)
	{
		drawMaskInverted(numerals_tiles_data[cardNumber], 5, x + CARD_WIDTH - CARD_NUMERAL_HOFFSET, y + CARD_HEIGHT - CARD_NUMERAL_VOFFSET);
		drawMaskInverted(small_suits_tiles_data[cardSuit], 4, x + CARD_WIDTH - CARD_FSUIT_HOFFSET, y + CARD_HEIGHT - CARD_FSUIT_VOFFSET);
	}

	if (cardNumber < 10)
	{
		// this is a number card (A-10) and should have pips
		const unsigned char *pipMask = medium_suits_tiles_data[cardSuit];
		unsigned char *codePtr = pipCode;
		for (unsigned char pipMap = segments[cardNumber]; pipMap != 0x00; pipMap <<= 1)
		{
			unsigned char counter = *codePtr & 0x0f;
			if (pipMap & 0x80)
			{
				const bool isNegative = *codePtr & 0x80;
				codePtr++;

				while (counter-- > 0)
				{
					if (isNegative) drawMaskInverted(pipMask, 6, x + codePtr[0], y + codePtr[1]);
					else drawMask(pipMask, 6, x + codePtr[0], y + codePtr[1]);

					codePtr += 2;
				}
			}
			else
			{
				codePtr += counter * 2 + 1;
			}
		}
	}
	else
	{
		gfx_Sprite(crown, x + 7, y + 17);
	}
}

void drawDeck()
{
	if (deckCards == 0) return;
	unsigned char y = DECK_VPOS;
	for (unsigned char x = 0; x <= deckCards; x += DECK_CARDS_PER_HEIGHT)
	{
		gfx_Sprite(card_back, DECK_HPOS, y);
		y -= 2;
	}
}

void drawStack(unsigned char stackIndex)
{
	for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
	{
		unsigned char cardX = TABL_HPOS + stackIndex * (CARD_WIDTH + CARD_SPACING);
		unsigned char cardY = TABL_VPOS + j * CARD_VOFFSET;
		
		drawCard(tableau[stackIndex][j], cardX, cardY, tableau[stackIndex][j + 1] & CARD_EXISTS);
	}
}

void drawBar()
{
	gfx_SetColor(BORDER_COLOR);
	gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, TOP_BORDER);

	gfx_SetTextFGColor(BLACK_COLOR);

	if (progress < PROGRESS_COMPLETE)
	{
		if (cursorMode == SELECT) gfx_PrintStringXY("SELECT", GFX_LCD_WIDTH / 2 - 3 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);
		else gfx_PrintStringXY("DROP", GFX_LCD_WIDTH / 2 - 2 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);
	}

	else gfx_PrintStringXY("COMPLETE", GFX_LCD_WIDTH / 2 - 4 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);

	gfx_SetTextXY(NUMWINS_DISP_X, SELCARD_DISP_Y);
	gfx_PrintUInt(numWins, 3);
}

void drawFrame(bool drawSelected)
{
	gfx_FillScreen(BKGND_COLOR);

	for (unsigned char i = 0; i < NUM_FREECELLS; i++) drawCard(freeCells[i], FC_HPOS + i * (CARD_WIDTH + CARD_SPACING), FC_VPOS, false);
	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++) drawStack(i);
	if (drawSelected && (selectedCard & CARD_EXISTS) && cursorMode == DROP) drawCard(selectedCard, SELCARD_XPOS, SELCARD_YPOS, false);

	drawDeck();
	drawBar();
	drawCursor();
	
	gfx_BlitBuffer();
}

void animateMove(unsigned int x0, unsigned char y0, unsigned int x1, unsigned char y1, bool faceDown)
{
	const bool flipX = x0 > x1;
	const bool flipY = y0 > y1;

	if (flipX)
	{
		x0 ^= x1;
		x1 ^= x0;
		x0 ^= x1;
	}
	if (flipY)
	{
		y0 ^= y1;
		y1 ^= y0;
		y0 ^= y1;
	}

	drawFrame(false);
	gfx_TempSprite(spriteBuffer, CARD_WIDTH, CARD_HEIGHT);

	const unsigned char Dy = y1 - y0;
	const clock_t duration = MOVE_ANIM_LENGTH * Dy;

	const clock_t startTime = clock();

	while (true)
	{
		const clock_t nowTime = clock();
		if (nowTime - startTime > duration) break;
		const clock_t elapsed = nowTime - startTime;

		const unsigned int dx = (x1 - x0) * elapsed / duration;
		const unsigned char dy = Dy * elapsed / duration;

		const unsigned int cardX = flipX ? x1 - dx : x0 + dx;
		const unsigned char cardY =  flipY ? y1 - dy : y0 + dy;

		gfx_GetSprite(spriteBuffer, cardX, cardY);
		if (faceDown)
		{
			gfx_Sprite(card_back, cardX, cardY);
		}
		else
		{
			drawCard(selectedCard, cardX, cardY, false);
		}
		gfx_BlitBuffer();
		gfx_Sprite(spriteBuffer, cardX, cardY);
	}
}