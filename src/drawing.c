#include "drawing.h"

#include <graphx.h>
#include "gfx/gfx.h"
#include "variables.h"

gfx_sprite_t* cardSprite[11];

void drawCursor(unsigned char X, unsigned char Y)
{
	if (cursorMode == SELECT) gfx_TransparentSprite(selcorner, X - 2, Y - 2);
	else gfx_TransparentSprite(drpcorner, X - 2, Y - 2);
}

void drawFrame()
{
	gfx_FillScreen(BKGND_COLOR);
	gfx_SetColor(BORDER_COLOR);
	gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, TOP_BORDER);

	for (unsigned char i = 0; i < NUM_FREECELLS; i++)
	{
		unsigned char cardX = FC_HPOS + i * (CARD_WIDTH + CARD_SPACING);

		if (freeCells[i] == 11) gfx_Sprite(freespace, cardX, FC_VPOS);
		else if (freeCells[i] > 11) gfx_Sprite(cardSprite[10], cardX, FC_VPOS);
		else gfx_Sprite(cardSprite[freeCells[i]], cardX, FC_VPOS);

		if (i >= fcUnlocked)
		{
			gfx_Sprite(lock, cardX + LOCK_ICON_HOFFSET, FC_VPOS + LOCK_ICON_VOFFSET);
		}

		if (i == cursorStack) drawCursor(cardX, FC_VPOS);
	}

	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
	{
		for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
		{
			unsigned char cardX = TABL_HPOS + i * (CARD_WIDTH + CARD_SPACING);
			unsigned char cardY = TABL_VPOS + j * CARD_VOFFSET;

			if (progress < 10) if (i + NUM_FREECELLS == cursorStack) if (j == cursorIndex)
				drawCursor(cardX, cardY);

			const unsigned char x = tableau[i][j];

			if (x == 11) break;
			if (x > 11) gfx_Sprite(cardSprite[10], cardX, cardY);
			else gfx_Sprite(cardSprite[x], cardX, cardY);
		}
	}

	if (progress < 10)
	{
		gfx_SetTextXY(SELCARD_DISP_X, SELCARD_DISP_Y);
		if (selectedCard != 11)
		{
			gfx_PrintUInt(selectedCard + 1, 1);
			if (selectedQty > 1)
			{
				gfx_PrintString(" x");
				gfx_PrintUInt(selectedQty, 1);
			}
		}
		else gfx_PrintString("EMPTY");
	}

	if (progress == 10)
		gfx_PrintStringXY("COMPLETE", GFX_LCD_WIDTH / 2 - 4 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);
	else if (cursorMode == SELECT) 
		gfx_PrintStringXY("SELECT", GFX_LCD_WIDTH / 2 - 3 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);
	else gfx_PrintStringXY("DROP", GFX_LCD_WIDTH / 2 - 2 * TEXT_CHAR_WIDTH, SELCARD_DISP_Y);

	gfx_SetTextXY(NUMWINS_DISP_X, SELCARD_DISP_Y);
	gfx_PrintUInt(numWins, 3);
	
	gfx_BlitBuffer();
}
