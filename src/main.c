#include <sys/rtc.h>
#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>

#include "gfx/gfx.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define SAVE_VAR_NAME "KBFDWINS"

#define BKGND_COLOR 2
#define BORDER_COLOR 1
#define TOP_BORDER 25

#define FRAME_TIME 3277
#define HOLD_TIME 2

#define CARD_HEIGHT card1_height
#define CARD_WIDTH card1_width
#define CARD_SPACING 5
#define CARD_VOFFSET 8
#define NUM_FREECELLS 4
#define NUM_TABLSLOTS 8
#define TABL_STACK_SIZE 16
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

gfx_sprite_t* cardSprite[11];

unsigned char tableau[NUM_TABLSLOTS][TABL_STACK_SIZE];
unsigned char freeCells[NUM_FREECELLS];
unsigned char fcUnlocked;

unsigned char cursorStack;
unsigned char cursorIndex;
enum { SELECT, DROP } cursorMode;
unsigned char selectedCard;
unsigned char selectedQty;
unsigned char orgStack;
unsigned char orgIndex;

unsigned char down, left, right, up;

unsigned char progress;
unsigned char numWins;

void start()
{
	srand(rtc_Time());

	// get number of wins
	numWins = 0;
	unsigned char saveHandle = ti_Open(SAVE_VAR_NAME, "r");
	if (saveHandle != 0) ti_Read(&numWins, 1, 1, saveHandle);
	ti_Close(saveHandle);

	// fill tableau and free cells with empty space
	for (unsigned char i = 0; i < NUM_FREECELLS; i++) 
		freeCells[i] = 11;
	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
		for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
			tableau[i][j] = 11;

	// deal the deck
	for (unsigned char i = 0; i < 10; i++)
	{
		for (unsigned char j = 0; j < 4; j++)
		{
			while (true)
			{
				unsigned char dStack = rand() % 8;
				unsigned char dIndex = rand() % 5;

				if (tableau[dStack][dIndex] == 11)
				{
					tableau[dStack][dIndex] = i;
					break;
				}
			}
		}
	}

	fcUnlocked = 1;
	progress = 0;

	cursorMode = SELECT;
	cursorStack = NUM_FREECELLS;
	cursorIndex = 40 / NUM_TABLSLOTS - 1;

	selectedCard = 1;
}

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

bool canGrabCard()
{
	if (cursorStack < NUM_FREECELLS) return freeCells[cursorStack] < 11;

	if (tableau[cursorStack - NUM_FREECELLS][0] > 11) return false;
	for (unsigned char i = cursorIndex + 1; i < TABL_STACK_SIZE; i++)
	{
		const unsigned char x = tableau[cursorStack - NUM_FREECELLS][i];
		if (x == 11) return true;
		if (x != selectedCard) return false;
	}

	return true;
}

bool canDropCard()
{
	if (cursorStack < NUM_FREECELLS) 
	{
		const unsigned char x = freeCells[cursorStack];
		if (cursorStack < fcUnlocked)
		{
			if (selectedQty == 1 || selectedQty == 4) return x == 11;
			else return x == selectedCard && selectedQty == 3;
		}
		else return false;
	}
	else if (cursorIndex == 0) return true;
	else return tableau[cursorStack - NUM_FREECELLS][cursorIndex - 1] == selectedCard;
}

bool checkTableauCollapse(unsigned char stackToCheck)
{
	const unsigned char x = tableau[stackToCheck][0];
	for (unsigned char i = 1; i < 5; i++)
	{
		const unsigned char y = tableau[stackToCheck][i];
		if (i == 4 && y == 11) return true;
		if (y != x) return false;
	}

	return false;
}

void dropCard()
{
	if (cursorStack == orgStack) return;

	unsigned char const prevProgress = progress;

	if (cursorStack < NUM_FREECELLS)
	{
		if (selectedQty == 1) freeCells[cursorStack] = selectedCard;
		else
		{
			progress++;
			freeCells[cursorStack] = 12 + selectedCard;
		}
	}
	else
	{
		for (unsigned char i = cursorIndex; i < cursorIndex + selectedQty; i++)
			tableau[cursorStack - NUM_FREECELLS][i] = selectedCard;
		if (checkTableauCollapse(cursorStack - NUM_FREECELLS))
		{
			fcUnlocked++;
			progress++;
			tableau[cursorStack - NUM_FREECELLS][0] = selectedCard + 12;
			for (int i = 1; i < 4; i++) tableau[cursorStack - NUM_FREECELLS][i] = 11;
		}
	}

	if (orgStack < NUM_FREECELLS) freeCells[orgStack] = 11;
	else for (unsigned char i = orgIndex; i < orgIndex + selectedQty; i++)
		tableau[orgStack - NUM_FREECELLS][i] = 11;

	if (progress == 10 && prevProgress < 10) numWins++;
}

void maxCursorIndex()
{
	if (cursorStack < NUM_FREECELLS) return;

	cursorIndex = TABL_STACK_SIZE - 1;
	while (tableau[cursorStack - NUM_FREECELLS][cursorIndex] == 11 && cursorIndex > 0) cursorIndex--;
}

bool cursorOnCollapsed()
{
	return cursorStack < NUM_FREECELLS ? freeCells[cursorStack] > 11 
									   : tableau[cursorStack - NUM_FREECELLS][0] > 11;
}

bool cursorOnLocked()
{
	return cursorStack >= fcUnlocked && cursorStack < NUM_FREECELLS;
}

bool doInput()
{
	const bool prevSecond = kb_IsDown(kb_Key2nd);
	const bool prevAlpha = kb_IsDown(kb_KeyAlpha);

	kb_Scan();

	up = kb_IsDown(kb_KeyUp) ? up + 1 : 0;
	down = kb_IsDown(kb_KeyDown) ? down + 1 : 0;
	left = kb_IsDown(kb_KeyLeft) ? left + 1 : 0;
	right = kb_IsDown(kb_KeyRight) ? right + 1 : 0;

	if (kb_IsDown(kb_Key2nd) && !prevSecond)
	{
		if (cursorMode == SELECT && canGrabCard())
		{
			orgStack = cursorStack;
			orgIndex = cursorIndex;
			cursorMode = DROP;
		}
		else if (cursorMode == DROP && canDropCard())
		{
			dropCard();
			cursorMode = SELECT;
		}
	}
	else if (kb_IsDown(kb_KeyAlpha) && !prevAlpha) cursorMode = SELECT;

	unsigned char prevCursorStack = cursorStack;

	if (down == 1 || down > HOLD_TIME)
	{
		cursorIndex++;
	}
	else if (left == 1 || left > HOLD_TIME)
	{
		do
		{
			if (cursorStack == 0) cursorStack = NUM_FREECELLS + NUM_TABLSLOTS - 1; // wrap
			else cursorStack--;
		}
		while (cursorOnCollapsed() || cursorOnLocked()); // skip useless stacks
	}
	else if (right == 1 || right > HOLD_TIME)
	{
		do
		{
			if (cursorStack > NUM_FREECELLS + NUM_TABLSLOTS - 2) cursorStack = 0; // wrap
			else cursorStack++;
		}
		while (cursorOnCollapsed() || cursorOnLocked()); // skip useless stacks
	}
	else if (up == 1 || up > HOLD_TIME)
	{
		if (cursorIndex > 0) cursorIndex--;
	}

	if (cursorStack < NUM_FREECELLS)
	{
		cursorIndex = 0;
	}
	else if (cursorMode == DROP)
	{
		maxCursorIndex();
		if (cursorIndex > 0) cursorIndex++;
	}
	else if (cursorStack != prevCursorStack)
	{
		maxCursorIndex();
	}
	else
	{
		while (tableau[cursorStack - NUM_FREECELLS][cursorIndex] == 11 && cursorIndex > 0) cursorIndex--;
	}

	if (cursorMode == SELECT)
	{
		selectedQty = 1;
		if (cursorStack < NUM_FREECELLS) selectedCard = freeCells[cursorStack];
		else 
		{
			selectedCard = tableau[cursorStack - NUM_FREECELLS][cursorIndex];
			for (int i = cursorIndex + 1; tableau[cursorStack - NUM_FREECELLS][i] == selectedCard; i++) 
				selectedQty++;
		}

		if (selectedCard > 11)
		{
			selectedCard -= 12;
			selectedQty = 4;
		}
	}

	return (!kb_IsDown(kb_KeyClear)) && (progress < 10);
}

bool run()
{
	start();

	while (doInput())
	{
		clock_t frameTimer = clock();

		drawFrame();

		while (clock() - frameTimer < FRAME_TIME);
	}

	if (progress == 10)
	{
		unsigned char const saveHandle = ti_Open(SAVE_VAR_NAME, "w");
		ti_Write(&numWins, 1, 1, saveHandle);
		ti_Close(saveHandle);

		while (kb_AnyKey());

		while (true)
		{
			kb_Scan();
			if (kb_IsDown(kb_KeyClear)) return false;
			if (kb_IsDown(kb_Key2nd)) return true;
		}
	}
}

int main(void)
{
	KBFDGFX_init();

	cardSprite[0] = card1;
	cardSprite[1] = card2;
	cardSprite[2] = card3;
	cardSprite[3] = card4;
	cardSprite[4] = card5;
	cardSprite[5] = card6;
	cardSprite[6] = card7;
	cardSprite[7] = card8;
	cardSprite[8] = card9;
	cardSprite[9] = card10;
	cardSprite[10] = cardstack;

	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
	gfx_SetTransparentColor(3);

	kb_EnableOnLatch();

	while (run());

	gfx_End();

	return 0;
}
