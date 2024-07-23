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

gfx_sprite_t* cardSprite[11];

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

void drawFrame()
{
	drawBackground();

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

void animateDeal()
{
	// phase 1: deck emerging from bottom of screen
	for (unsigned char deckY = GFX_LCD_HEIGHT; deckY >= DEAL_ANIM_DECK_Y; deckY--)
	{
		clock_t frameTimer = clock();

		drawBackground();

		gfx_Sprite(cardSprite[10], DEAL_ANIM_DECK_X, deckY);

		while (clock() - frameTimer < DEAL_ANIM_DECK_FRAME_TIME);

		gfx_BlitBuffer();
	}

	clock_t mainTimer = clock();
	while (clock() - mainTimer < DEAL_ANIM_DECK_PAUSE_TIME);

	// phase 2: cards interpolating between deck and end positions
	mainTimer = clock();
	bool deckExists = true;
	while (clock() - mainTimer < DEAL_ANIM_TOTAL_TIME)
	{
		clock_t frameTimer = clock();

		drawBackground();

		for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
		{
			for (unsigned char j = 0; tableau[i][j] != 11; j++)
			{
				// should this card have even launched yet?
				const unsigned int delay = (i + j * 8) * DEAL_ANIM_LAUNCH_INTERVAL;
				if (clock() - mainTimer <= delay) continue;

				// if last card launched, deck sprite disappears
				if (i == 7 && j == 4) deckExists = false;

				// where is the card going?
				unsigned char cardEndX = TABL_HPOS + i * (CARD_WIDTH + CARD_SPACING);
				unsigned char cardEndY = TABL_VPOS + j * CARD_VOFFSET;

				unsigned char cardX, cardY;

				// is this card done traveling yet?
				if (clock() - mainTimer >= delay + DEAL_ANIM_TRAVEL_TIME)
				{
					cardX = cardEndX;
					cardY = cardEndY;
				}
				else
				{
					// okay we need to actually interpolate
					const signed int travelingFor = clock() - mainTimer - delay;
					const signed char dX = cardEndX - DEAL_ANIM_DECK_X;
					const signed char dY = cardEndY - DEAL_ANIM_DECK_Y;
					cardX = DEAL_ANIM_DECK_X + travelingFor * dX / DEAL_ANIM_TRAVEL_TIME;
					cardY = DEAL_ANIM_DECK_Y + travelingFor * dY / DEAL_ANIM_TRAVEL_TIME;
				}

				gfx_Sprite(cardSprite[tableau[i][j]], cardX, cardY);
			}
		}

		if (deckExists) gfx_Sprite(cardSprite[10], DEAL_ANIM_DECK_X, DEAL_ANIM_DECK_Y);

		while (clock() - frameTimer < DEAL_ANIM_FRAME_TIME);

		gfx_BlitBuffer();
	}
}
